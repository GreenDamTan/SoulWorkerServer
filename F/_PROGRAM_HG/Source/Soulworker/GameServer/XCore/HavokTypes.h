// HavokTypes.h
// Havok Physics/AI Engine Types - Stub definitions for compilation
// Reconstructed from IDA decompilation of GameServer.exe

#pragma once

#include <cstdint>
#include <cstddef>     // For std::nullptr_t
#include <vector>
#include <cmath>
#include <cstring>     // For strlen, strcpy, strcmp, etc.
#include <cstdarg>     // For va_list

#ifdef _WIN32
#include <xmmintrin.h>  // SSE intrinsics for _mm_unpacklo_ps
#include <emmintrin.h>  // SSE2 intrinsics for __m128i
#include <mmintrin.h>   // MMX intrinsics for _mm_storel_pi
#include <windows.h>    // For TlsGetValue
#endif

// Forward declarations
class hkaiWorld;
class hkaiNavMeshInstance;
class hkaiNavMeshQueryMediator;
class HavokNavMeshResource;
class hkClass;
class hkBaseObject;
class hkMemoryRouter;
class hkVector4;
class hkRotation;
class hkTransform;
class hkQsTransform;
class hkQTransform;
class hkMatrix3;
class hkSimdReal;
class hkLocalFrame;
class hkLocalFrameGroup;
class hkLocalFrameCollector;
class hkMemoryTrack;
class hkBufferedStreamWriter;
class hkMemoryTrackStreamWriter;

// Forward template declaration
template<typename T> class hkSingleton;

// ============================================================================
// Havok Global Constants
// ============================================================================

// IDA: g_vectorConstants - Global vector constants table
// Used by hkVector4::getConstant<N>() template functions
extern "C" const float g_vectorConstants[256];

// IDA: _mask__NegFloat_ - Mask for negating floats via XOR
extern "C" const __m128 _mask__NegFloat_;

// ============================================================================
// Havok Common Types
// ============================================================================

// HK_SUCCESS - Havok success constant
#define HK_SUCCESS 0

// hkResult - Result code for Havok operations
struct hkResult {
    union {
        int m_result;  // 0 = success, non-zero = failure
        int m_enum;    // Alias for m_result (used in IDA)
    };

    hkResult() : m_result(0) {}
    hkResult(int r) : m_result(r) {}

    bool isSuccess() const { return m_result == HK_SUCCESS; }
    operator bool() const { return isSuccess(); }
};

// hkBool - Boolean wrapper
struct hkBool {
    bool m_bool;

    hkBool() : m_bool(false) {}
    hkBool(bool b) : m_bool(b) {}
    operator bool() const { return m_bool; }
};

template<typename EnumType, typename StorageType>
struct hkFlags {
    StorageType m_storage;
    hkFlags() : m_storage(0) {}
    hkFlags(StorageType val) : m_storage(val) {}
};

// ============================================================================
// Havok Memory Management
// ============================================================================

// hkMemUtil - Memory utility namespace
namespace hkMemUtil {
    // IDA: memCpy wrapper
    inline void memCpy(void* dst, const void* src, int size) {
        memcpy(dst, src, size);
    }

    // IDA: memMove wrapper
    inline void memMove(void* dst, const void* src, int size) {
        memmove(dst, src, size);
    }

    // IDA: ?memSet@hkMemUtil@@YAXPEAXHH@Z @ 0x140793000
    inline void memSet(void* dst, int val, int size) {
        memset(dst, val, size);
    }

    // IDA: ?memCpyBackwards@hkMemUtil@@YAXPEAXPEBXH@Z @ 0x140793110
    // Copy backwards (for overlapping regions where dst > src)
    inline void memCpyBackwards(void* dst, const void* src, int size) {
        const char* s = static_cast<const char*>(src) + size - 1;
        char* d = static_cast<char*>(dst) + size - 1;
        for (int i = 0; i < size; i++) {
            *d-- = *s--;
        }
    }
}

// hkMemoryAllocator - Memory allocator base
// IDA: Virtual function table with blockAlloc, blockFree methods
class hkMemoryAllocator {
public:
    // MemoryStatistics structure for tracking memory usage
    struct MemoryStatistics {
        int m_allocated;
        int m_used;
        int m_peakUsed;
        int m_totalAllocated;
        int m_peakInUse;  // IDA: peak memory in use
    };

    // IDA: ??1hkMemoryAllocator@@UEAA@XZ @ 0x14078e070
    virtual ~hkMemoryAllocator() {}

    // IDA: ?blockAlloc@hkMemoryAllocator@@UEAAPEAXH@Z
    // Virtual block allocation - allocates a block of given size
    virtual void* blockAlloc(int numBytes) {
        return ::operator new(numBytes);
    }

    // IDA: ?blockFree@hkMemoryAllocator@@UEAAXPEAXH@Z @ 0x1402f8920
    // Virtual block free - frees a previously allocated block
    virtual void blockFree(void* p, int size) {
        ::operator delete(p);
    }

    // IDA: ?bufAlloc@hkMemoryAllocator@@UEAAPEAXAEAH@Z @ 0x14078e090
    virtual void* bufAlloc(int& reqBytesInOut) {
        return blockAlloc(reqBytesInOut);
    }

    // IDA: ?bufFree@hkMemoryAllocator@@UEAAXPEAXH@Z @ 0x14078e0a0
    virtual void bufFree(void* p, int numBytes) {
        blockFree(p, numBytes);
    }

    // Template helpers for typed buffer allocation
    // IDA: ??$_bufAlloc@VhkVector4@@@hkMemoryAllocator@@QEAAPEAVhkVector4@@AEAH@Z @ 0x1402f8bb0
    template<typename T>
    T* _bufAlloc(int* reqNumInOut) {
        int nBytes = sizeof(T) * (*reqNumInOut);
        T* result = static_cast<T*>(bufAlloc(nBytes));
        *reqNumInOut = nBytes / sizeof(T);
        return result;
    }

    // IDA: ??$_bufFree@VhkVector4@@@hkMemoryAllocator@@QEAAXPEAXH@Z @ 0x140276f50
    template<typename T>
    void _bufFree(T* p, int numElements) {
        bufFree(p, sizeof(T) * numElements);
    }

    // IDA: ?blockAllocBatch@hkMemoryAllocator@@UEAAXPEAPEAXHH@Z @ 0x14078e0b0
    virtual void blockAllocBatch(void** ptrsOut, int numPtrs, unsigned int blockSize) {
        for (int i = 0; i < numPtrs; i++) {
            ptrsOut[i] = blockAlloc(blockSize);
        }
    }

    // IDA: ?blockFreeBatch@hkMemoryAllocator@@UEAAXPEAPEAXHH@Z @ 0x14078e120
    virtual void blockFreeBatch(void** ptrsIn, unsigned int numPtrs, int blockSize) {
        for (unsigned int i = 0; i < numPtrs; i++) {
            if (ptrsIn[i]) {
                blockFree(ptrsIn[i], blockSize);
            }
        }
    }

    // IDA: ?bufRealloc@hkMemoryAllocator@@UEAAPEAXPEAXHAEAH@Z @ 0x14078e190
    virtual void* bufRealloc(void* pold, int oldNumBytes, int* reqBytesInOut) {
        void* newPtr = bufAlloc(*reqBytesInOut);
        if (newPtr) {
            int copySize = oldNumBytes < *reqBytesInOut ? oldNumBytes : *reqBytesInOut;
            hkMemUtil::memCpy(newPtr, pold, copySize);
        }
        bufFree(pold, oldNumBytes);
        return newPtr;
    }

    // IDA: ?getMemoryStatistics@hkMemoryAllocator@@UEAAXAEAUMemoryStatistics@1@@Z
    virtual void getMemoryStatistics(MemoryStatistics& u) {
        u.m_allocated = 0;
        u.m_used = 0;
        u.m_peakUsed = 0;
        u.m_totalAllocated = 0;
    }

    // IDA: ?getAllocatedSize@hkMemoryAllocator@@UEAAHPEBXH@Z
    virtual int getAllocatedSize(const void* obj, int nbytes) {
        return nbytes;
    }

    // IDA: ??$_bufAlloc@VhkVector4@@@hkMemoryAllocator@@QEAAPEAVhkVector4@@AEAH@Z
    template<typename T>
    T* bufAlloc(int& n) {
        // Stub - use standard allocator
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    // IDA: ??$_bufFree@VhkVector4@@@hkMemoryAllocator@@QEAAXPEAXH@Z
    template<typename T>
    void bufFree(void* p, int n) {
        ::operator delete(p);
    }
};

// hkMallocAllocator - Global malloc allocator
// IDA: Used by hkContainerDefaultMallocAllocator
class hkMallocAllocator : public hkMemoryAllocator {
public:
    int m_currentUsed;   // IDA: current memory usage
    int m_peakUse;       // IDA: peak memory usage
    int m_align;         // IDA: alignment

    hkMallocAllocator()
        : m_currentUsed(0)
        , m_peakUse(0)
        , m_align(16) {
    }

    static hkMallocAllocator* m_defaultMallocAllocator;

    // IDA: ?blockAlloc@hkMallocAllocator@@UEAAPEAXH@Z @ 0x14078f450
    void* blockAlloc(int numBytes) override;

    // IDA: ?getMemoryStatistics@hkMallocAllocator@@UEAAXAEAUMemoryStatistics@hkMemoryAllocator@@@Z @ 0x14078f4a0
    void getMemoryStatistics(MemoryStatistics& u) override;

    // IDA: ?blockFree@hkMallocAllocator@@UEAAXPEAXH@Z @ 0x14078f480
    void blockFree(void* p, int numBytes) override;

    // IDA: ?resetPeakMemoryStatistics@hkMallocAllocator@@UEAAXXZ @ 0x14078f4c0
    void resetPeakMemoryStatistics() { m_peakUse = m_currentUsed; }

    // IDA: ?getAllocatedSize@hkMallocAllocator@@UEAAHPEBXH@Z @ 0x14078f4d0
    int getAllocatedSize(const void* obj, int numBytes) override { return numBytes; }
};

// hkContainerDefaultMallocAllocator - Default malloc allocator wrapper
// IDA: ?get@hkContainerDefaultMallocAllocator@@QEAAAEAVhkMemoryAllocator@@PEBX@Z @ 0x14078ea40
class hkContainerDefaultMallocAllocator {
public:
    hkMemoryAllocator* get(const void* = nullptr) {
        return hkMallocAllocator::m_defaultMallocAllocator;
    }
};

// hkContainerHeapAllocator - Heap allocator for containers
// IDA: ?get@hkContainerHeapAllocator@@QEAAAEAVhkMemoryAllocator@@PEBX@Z @ 0x140277720
class hkContainerHeapAllocator {
public:
    struct Allocator : public hkMemoryAllocator {
        // IDA: ?blockAlloc@Allocator@hkContainerHeapAllocator@@UEAAPEAXH@Z @ 0x14078ed90
        void* blockAlloc(int numBytes) override;

        // IDA: ?blockFree@Allocator@hkContainerHeapAllocator@@UEAAXPEAXH@Z @ 0x14078edc0
        void blockFree(void* p, int numBytes) override;

        // IDA: ?bufAlloc@Allocator@hkContainerHeapAllocator@@UEAAPEAXAEAH@Z @ 0x14078ee00
        void* bufAlloc(int& reqNumBytesInOut) override;

        // IDA: ?bufFree@Allocator@hkContainerHeapAllocator@@UEAAXPEAXH@Z @ 0x14078ee30
        void bufFree(void* p, int numBytes) override;

        // IDA: ?bufRealloc@Allocator@hkContainerHeapAllocator@@UEAAPEAXPEAXHAEAH@Z @ 0x14078ee70
        void* bufRealloc(void* pold, int oldNumBytes, int* reqNumBytesInOut) override;

        // IDA: ?getMemoryStatistics@Allocator@hkContainerHeapAllocator@@UEAAXAEAUMemoryStatistics@hkMemoryAllocator@@@Z @ 0x14078eec0
        void getMemoryStatistics(MemoryStatistics& u) override;

        // IDA: ?getAllocatedSize@Allocator@hkContainerHeapAllocator@@UEAAHPEBXH@Z @ 0x14078eef0
        int getAllocatedSize(const void* obj, int nbytes) override;
    };
    static Allocator s_alloc;

    Allocator& get(const void* = nullptr) {
        return s_alloc;
    }
};

// hkContainerTempAllocator - Temporary allocator for containers
// Used for short-lived allocations during initialization
// IDA: Uses hkMemoryRouter::getInstance()->m_temp for allocations
class hkContainerTempAllocator {
public:
    struct Allocator : public hkMemoryAllocator {
        // IDA: ?blockAlloc@Allocator@hkContainerTempAllocator@@UEAAPEAXH@Z @ 0x14078ea50
        void* blockAlloc(int numBytes) override;

        // IDA: ?blockFree@Allocator@hkContainerTempAllocator@@UEAAXPEAXH@Z @ 0x14078ea80
        void blockFree(void* p, int numBytes) override;

        // IDA: ?bufAlloc@Allocator@hkContainerTempAllocator@@UEAAPEAXAEAH@Z @ 0x14078eac0
        void* bufAlloc(int& reqNumBytesInOut) override;

        // IDA: ?bufFree@Allocator@hkContainerTempAllocator@@UEAAXPEAXH@Z @ 0x14078eaf0
        void bufFree(void* p, int numBytes) override;

        // IDA: ?bufRealloc@Allocator@hkContainerTempAllocator@@UEAAPEAXPEAXHAEAH@Z @ 0x14078eb30
        void* bufRealloc(void* pold, int oldNumBytes, int* reqNumBytesInOut) override;

        // IDA: ?getMemoryStatistics@Allocator@hkContainerTempAllocator@@UEAAXAEAUMemoryStatistics@hkMemoryAllocator@@@Z @ 0x14078eb80
        void getMemoryStatistics(MemoryStatistics& u) override;

        // IDA: ?getAllocatedSize@Allocator@hkContainerTempAllocator@@UEAAHPEBXH@Z @ 0x14078ebb0
        int getAllocatedSize(const void* obj, int nbytes) override;
    };
    static Allocator s_alloc;

    Allocator& get(const void* = nullptr) {
        return s_alloc;
    }
};

// hkContainerDebugAllocator - Debug allocator for containers
// Uses hkMemoryRouter::m_debug for allocations
class hkContainerDebugAllocator {
public:
    struct Allocator : public hkMemoryAllocator {
        // IDA: ?blockAlloc@Allocator@hkContainerDebugAllocator@@UEAAPEAXH@Z @ 0x14078ebf0
        void* blockAlloc(int numBytes) override;

        // IDA: ?blockFree@Allocator@hkContainerDebugAllocator@@UEAAXPEAXH@Z @ 0x14078ec20
        void blockFree(void* p, int numBytes) override;

        // IDA: ?bufAlloc@Allocator@hkContainerDebugAllocator@@UEAAPEAXAEAH@Z @ 0x14078ec60
        void* bufAlloc(int& reqNumBytesInOut) override;

        // IDA: ?bufFree@Allocator@hkContainerDebugAllocator@@UEAAXPEAXH@Z @ 0x14078ec90
        void bufFree(void* p, int numBytes) override;

        // IDA: ?bufRealloc@Allocator@hkContainerDebugAllocator@@UEAAPEAXPEAXHAEAH@Z @ 0x14078ecd0
        void* bufRealloc(void* pold, int oldNumBytes, int* reqNumBytesInOut) override;

        // IDA: ?getMemoryStatistics@Allocator@hkContainerDebugAllocator@@UEAAXAEAUMemoryStatistics@hkMemoryAllocator@@@Z @ 0x14078ed20
        void getMemoryStatistics(MemoryStatistics& u) override;

        // IDA: ?getAllocatedSize@Allocator@hkContainerDebugAllocator@@UEAAHPEBXH@Z @ 0x14078ed50
        int getAllocatedSize(const void* obj, int nbytes) override;
    };
    static Allocator s_alloc;

    Allocator& get(const void* = nullptr) {
        return s_alloc;
    }
};
// IDA: inherits from hkMemoryAllocator
class hkLifoAllocator : public hkMemoryAllocator {
public:
    // NonLifoFree entry - tracks non-LIFO allocations
    struct NonLifoFree {
        char* start;
        char* end;
        int slabIndex;
    };

    // hkArray-like structure for internal arrays
    template<typename T>
    struct Array {
        T* m_data;
        int m_size;
        int m_capacityAndFlags;
    };

    // Implementation struct for external allocation tracking
    struct Implementation {
        Array<NonLifoFree> m_nonLifoFrees;
        Array<void*> m_slabPtrs;
        int m_numExternalAllocations;
    };

    // Additional members beyond base class
    hkMemoryAllocator* m_slabAllocator;
    hkMemoryAllocator* m_largeAllocator;
    Implementation* m_impl;
    char* m_cur;
    char* m_end;
    char* m_firstNonLifoEnd;
    void* m_cachedEmptySlab;
    hkMemoryAllocator* m_internalAllocator;
    int m_slabSize;

    // IDA: ??0hkLifoAllocator@@QEAA@H@Z @ 0x140792150
    hkLifoAllocator(int slabSize = 0)
        : m_slabAllocator(nullptr), m_largeAllocator(nullptr), m_impl(nullptr),
          m_cur(nullptr), m_end(nullptr), m_firstNonLifoEnd(nullptr),
          m_cachedEmptySlab(nullptr), m_internalAllocator(nullptr), m_slabSize(slabSize) {}

    // IDA: ??1hkLifoAllocator@@UEAA@XZ @ 0x1401e83d0
    virtual ~hkLifoAllocator() {}

    // IDA: ?init@hkLifoAllocator@@QEAAXPEAVhkMemoryAllocator@@00@Z @ 0x140792170
    void init(hkMemoryAllocator* slabAllocator, hkMemoryAllocator* largeAllocator, hkMemoryAllocator* internalAllocator);

    // IDA: ?quit@hkLifoAllocator@@QEAAXQEAPEAVhkMemoryAllocator@@@Z @ 0x140792410
    void quit(hkMemoryAllocator** allocators);

    // IDA: ?blockAlloc@hkLifoAllocator@@UEAAPEAXH@Z @ 0x140792870
    void* blockAlloc(int numBytesIn) override {
        int alignedSize = (numBytesIn + 15) & ~15;  // Align to 16 bytes
        char* result = m_cur;
        char* newCur = result + alignedSize;

        if (alignedSize > m_slabSize || newCur > m_end) {
            return allocateFromNewSlab(alignedSize);
        }
        m_cur = newCur;
        return result;
    }

    // IDA: ?blockFree@hkLifoAllocator@@UEAAXPEAXH@Z @ 0x1407928b0
    void blockFree(void* p, int numBytesIn) override {
        int alignedSize = (numBytesIn + 15) & ~15;
        char* ptr = static_cast<char*>(p);

        if (alignedSize > m_slabSize || ptr + alignedSize != m_cur || m_firstNonLifoEnd == ptr) {
            slowBlockFree(ptr, alignedSize);
        } else {
            m_cur = ptr;
        }
    }

    // IDA: ?bufAlloc@hkLifoAllocator@@UEAAPEAXAEAH@Z @ 0x1407928f0
    void* bufAlloc(unsigned int& reqNumBytesInOut) {
        unsigned int v2 = (reqNumBytesInOut + 15) & 0xFFFFFFF0;
        reqNumBytesInOut = v2;
        char* m_cur_local = m_cur;
        int v4 = (v2 + 15) & 0xFFFFFFF0;
        char* v5 = m_cur_local + v4;
        if (v4 > m_slabSize || v5 > m_end) {
            return allocateFromNewSlab(v4);
        }
        m_cur = v5;
        return m_cur_local;
    }

    // IDA: ?bufFree@hkLifoAllocator@@UEAAXPEAXH@Z @ 0x140792930
    void bufFree(void* p, int numBytes) override {
        int v3 = (numBytes + 15) & 0xFFFFFFF0;
        char* ptr = static_cast<char*>(p);
        if (numBytes > m_slabSize || ptr + v3 != m_cur || m_firstNonLifoEnd == ptr) {
            slowBlockFree(ptr, v3);
        } else {
            m_cur = ptr;
        }
    }

    // IDA: ?bufRealloc@hkLifoAllocator@@UEAAPEAXPEAXHAEAH@Z @ 0x140792210
    void* bufRealloc(void* pold, int oldNumBytes, int* reqNumBytesInOut) override;

    // IDA: ?numExternalAllocations@hkLifoAllocator@@QEBAHXZ @ 0x140792130
    int numExternalAllocations() const {
        return m_impl ? m_impl->m_numExternalAllocations : 0;
    }

    // IDA: ?isEmpty@hkLifoAllocator@@QEBAIXZ @ 0x1407922c0
    unsigned int isEmpty() const {
        if (!m_impl) return 1;
        // Simplified check: if we have no impl, we're empty
        return 1;
    }

    // IDA: ?allocateFromNewSlab@hkLifoAllocator@@IEAAPEAXH@Z @ 0x140792520
    void* allocateFromNewSlab(int size) {
        // Stub - would allocate new slab from heap allocator
        return ::operator new(size);
    }

    // IDA: ?popNonLifoFrees@hkLifoAllocator@@IEAAXXZ @ 0x1407922e0
    void popNonLifoFrees();

    // IDA: ?insertNonLifoFree@hkLifoAllocator@@IEAAXPEAXH@Z @ 0x140792660
    void insertNonLifoFree(char* pstart, int nbytes);

    // IDA: ?slowBlockFree@hkLifoAllocator@@IEAAXPEAXH@Z @ 0x140792810
    void slowBlockFree(void* p, int size) {
        // Stub - handles non-LIFO free cases
        ::operator delete(p);
    }
};

// hkThreadLocalData - Thread-local storage wrapper
// IDA: ?getData@?$hkThreadLocalData@PEAVhkMemoryRouter@@@@QEBAPEAVhkMemoryRouter@@XZ @ 0x1401adf60
template<typename T>
class hkThreadLocalData {
public:
    unsigned long m_slotID;

    // IDA: getData - returns thread-local value via TLS
    T getData() const {
#ifdef _WIN32
        return reinterpret_cast<T>(TlsGetValue(m_slotID));
#else
        return T{}; // Stub for non-Windows
#endif
    }
};

// hkMemoryRouter - Memory router for Havok allocations
// IDA: Multiple functions at 0x14078bxxx
class hkMemoryRouter {
public:
    hkLifoAllocator m_stack;           // IDA: m_stack member at offset 0
    hkMemoryAllocator* m_temp;         // IDA: m_temp member - resetPeakMemoryStatistics @ 0x14078b4b0
    hkMemoryAllocator* m_heap;         // IDA: m_heap member - returned by "size_" function @ 0x140140280
    hkMemoryAllocator* m_debug;        // IDA: m_debug member - resetPeakMemoryStatistics @ 0x14078b4b0
    hkMemoryAllocator* m_solver;       // IDA: m_solver member - resetPeakMemoryStatistics @ 0x14078b4b0
    int m_refObjLocalStore;            // IDA: m_refObjLocalStore @ 0x14078b510
    static hkThreadLocalData<hkMemoryRouter*> s_memoryRouter;

    // IDA: ??0hkMemoryRouter@@QEAA@XZ @ 0x14078b510
    hkMemoryRouter() : m_stack(0x8000), m_temp(nullptr), m_heap(nullptr),
                       m_debug(nullptr), m_solver(nullptr), m_refObjLocalStore(0) {}

    // IDA: ??1hkMemoryRouter@@QEAA@XZ @ 0x1401e83f0
    ~hkMemoryRouter() {
        // Destructor calls m_stack destructor
    }

    // IDA: ?getInstance@hkMemoryRouter@@SAAEAV1@XZ @ 0x1401adf80
    static hkMemoryRouter* getInstance() {
        return s_memoryRouter.getData();
    }

    // IDA: ?alignedAlloc@hkMemoryRouter@@SAPEAXAEAVhkMemoryAllocator@@HH@Z @ 0x14078b380
    // Allocates aligned memory with header containing allocator ptr, size, and offset
    // Header layout at result-16: [allocator* (8)] [size (4)] [offset (4)]
    static void* alignedAlloc(hkMemoryAllocator& b, int nbytes, int alignment) {
        int totalSize = nbytes + alignment;
        int allocSize = nbytes + alignment + 16;
        char* raw = static_cast<char*>(b.blockAlloc(allocSize));
        // Align up to boundary, leaving room for 16-byte header
        uintptr_t aligned = (reinterpret_cast<uintptr_t>(raw) + alignment + 15) & ~static_cast<uintptr_t>(alignment - 1);
        char* result = reinterpret_cast<char*>(aligned);
        // Store header: allocator ptr at result-16, size at result-8, offset at result-4
        *reinterpret_cast<hkMemoryAllocator**>(result - 16) = &b;
        *reinterpret_cast<int*>(result - 8) = totalSize;
        *reinterpret_cast<int*>(result - 4) = static_cast<int>(result - raw);
        return result;
    }

    // IDA: ?alignedFree@hkMemoryRouter@@SAXAEAVhkMemoryAllocator@@PEAX@Z @ 0x14078b3e0
    static void alignedFree(hkMemoryAllocator& b, void* p) {
        if (p) {
            char* ptr = static_cast<char*>(p);
            int totalSize = *reinterpret_cast<int*>(ptr - 8);
            int offset = *reinterpret_cast<int*>(ptr - 4);
            // Clear the allocator pointer (optional safety)
            *reinterpret_cast<hkMemoryAllocator**>(ptr - 16) = nullptr;
            // Free the original allocation
            b.blockFree(ptr - offset, totalSize + 16);
        }
    }

    // IDA: ?easyAlloc@hkMemoryRouter@@SAPEAXAEAVhkMemoryAllocator@@H@Z @ 0x14078b420
    // Allocates memory with 16-byte header: [allocator* (8)] [unused (4)] [size (4)]
    static void* easyAlloc(hkMemoryAllocator& b, int nbytes) {
        char* raw = static_cast<char*>(b.blockAlloc(nbytes + 16));
        // Store allocator at offset 0, size at offset 8
        *reinterpret_cast<hkMemoryAllocator**>(raw) = &b;
        *reinterpret_cast<int*>(raw + 8) = nbytes;
        // Return pointer after 16-byte header
        return raw + 16;
    }

    // IDA: ?getEasyAllocSize@hkMemoryRouter@@SA_KAEAVhkMemoryAllocator@@PEBX@Z @ 0x14078b460
    static size_t getEasyAllocSize(hkMemoryAllocator& b, const void* ptr) {
        const char* p = static_cast<const char*>(ptr);
        return *reinterpret_cast<const int*>(p - 8);
    }

    // IDA: ?getEasyAllocStartAddress@hkMemoryRouter@@SAPEBXAEAVhkMemoryAllocator@@PEBX@Z @ 0x14078b470
    static const void* getEasyAllocStartAddress(hkMemoryAllocator& b, const void* ptr) {
        return static_cast<const char*>(ptr) - 16;
    }

    // IDA: ?easyFree@hkMemoryRouter@@SAXAEAVhkMemoryAllocator@@PEAX@Z @ 0x14078b480
    static void easyFree(hkMemoryAllocator& b, void* p) {
        if (p) {
            char* ptr = static_cast<char*>(p);
            int nbytes = *reinterpret_cast<int*>(ptr - 8);
            char* raw = ptr - 16;
            // Clear header (optional safety)
            *reinterpret_cast<uintptr_t*>(raw) = 0;
            b.blockFree(raw, nbytes + 16);
        }
    }

    // IDA: ?resetPeakMemoryStatistics@hkMemoryRouter@@QEAAXXZ @ 0x14078b4b0
    void resetPeakMemoryStatistics() {
        if (m_debug) m_debug->blockFree(nullptr, 0);  // Placeholder - actually calls resetPeakMemoryStatistics
        if (m_heap) m_heap->blockFree(nullptr, 0);
        if (m_solver) m_solver->blockFree(nullptr, 0);
        if (m_temp) m_temp->blockFree(nullptr, 0);
    }

    // IDA: ?replaceInstance@hkMemoryRouter@@SAXPEAV1@@Z @ 0x14078b4f0
    static void replaceInstance(hkMemoryRouter* a) {
#ifdef _WIN32
        TlsSetValue(s_memoryRouter.m_slotID, a);
#endif
    }

    // getCurrentThreadAllocator - Get the heap allocator for the current thread
    // Used by hkString::strDup, strFree, strNdup without explicit allocator
    static hkMemoryAllocator* getCurrentThreadAllocator() {
        hkMemoryRouter* router = getInstance();
        if (router) {
            return router->m_heap;
        }
        return nullptr;
    }
};

// hkMemorySystem - Memory system
class hkMemorySystem {
public:
    enum FlagBits {
        FLAG_PERSISTENT = 1,
        FLAG_TEMPORARY = 2,
        FLAG_ALL = 3
    };
    using Flags = hkFlags<FlagBits, int>;

    // FrameInfo structure for memory system initialization
    struct FrameInfo {
        int m_solverBufferSize;

        // IDA: ??0FrameInfo@hkMemorySystem@@QEAA@H@Z @ 0x14078e230
        FrameInfo(int solverBufferSize = 0) : m_solverBufferSize(solverBufferSize) {}
    };

    // Static instance
    static hkMemorySystem* s_instance;

    // IDA: ?replaceInstance@hkMemorySystem@@SAXPEAV1@@Z @ 0x14078e240
    static void replaceInstance(hkMemorySystem* m) { s_instance = m; }

    // IDA: ?getInstance@hkMemorySystem@@SAAEAV1@XZ @ 0x14078e250
    static hkMemorySystem& getInstance() { return *s_instance; }

    // IDA: ?getInstancePtr@hkMemorySystem@@SAPEAV1@XZ @ 0x14078e260
    static hkMemorySystem* getInstancePtr() { return s_instance; }

    // IDA: ??1hkMemorySystem@@UEAA@XZ @ 0x14078e270
    virtual ~hkMemorySystem() {}

    // IDA: ?mainInit@hkMemorySystem@@UEAAPEAVhkMemoryRouter@@AEBUFrameInfo@1@V?$hkFlags@W4FlagBits@1@@H@@Z
    virtual hkMemoryRouter* mainInit(const FrameInfo&, Flags) {
        static hkMemoryRouter mainRouter;
        threadInit(mainRouter, "main", Flags(FLAG_ALL));
        return &mainRouter;
    }

    // IDA: ?mainQuit@hkMemorySystem@@UEAA?AUhkResult@@V?$hkFlags@W4FlagBits@1@@Z
    virtual hkResult mainQuit(Flags) { return hkResult(HK_SUCCESS); }

    // IDA: ?threadInit@hkMemorySystem@@UEAAXAEAVhkMemoryRouter@@PEBDV?$hkFlags@W4FlagBits@1@@H@@Z
    virtual void threadInit(hkMemoryRouter& router, const char*, Flags) {
        hkMemoryAllocator* allocator = hkMallocAllocator::m_defaultMallocAllocator;
        router.m_temp = allocator;
        router.m_heap = allocator;
        router.m_debug = allocator;
        router.m_solver = allocator;
    }

    // IDA: ?threadQuit@hkMemorySystem@@UEAAXAEAVhkMemoryRouter@@V?$hkFlags@W4FlagBits@1@@H@@Z
    virtual void threadQuit(hkMemoryRouter& router, Flags) {
        router.m_temp = nullptr;
        router.m_heap = nullptr;
        router.m_debug = nullptr;
        router.m_solver = nullptr;
    }

    // Virtual methods
    // IDA: ?isOk@hkMemorySystem@@UEBAIXZ @ 0x14004dc20
    virtual unsigned int isOk() const { return 1; }

    // IDA: ?garbageCollectThread@hkMemorySystem@@UEAAXAEAVhkMemoryRouter@@@Z @ 0x14078e290
    virtual void garbageCollectThread(hkMemoryRouter&) {}

    // IDA: ?garbageCollectShared@hkMemorySystem@@UEAAXXZ @ 0x14078e2a0
    virtual void garbageCollectShared() {}

    // IDA: ?setHeapSoftLimit@hkMemorySystem@@UEAA?AUhkResult@@H@Z @ 0x14078e2b0
    virtual hkResult setHeapSoftLimit(int) { return hkResult(0); }

    // IDA: ?getHeapSoftLimit@hkMemorySystem@@UEBAHXZ @ 0x14078e2c0
    virtual int getHeapSoftLimit() const { return 0; }

    // IDA: ?solverCanAllocSingleBlock@hkMemorySystem@@UEAA_NH@Z @ 0x14078e2d0
    virtual bool solverCanAllocSingleBlock(int) { return true; }

    // IDA: ?heapCanAllocTotal@hkMemorySystem@@UEAA_NH@Z @ 0x14078e2e0
    virtual bool heapCanAllocTotal(int) { return true; }

    // IDA: ?garbageCollect@hkMemorySystem@@UEAAXXZ @ 0x14078e2f0
    virtual void garbageCollect() {}

    // IDA: ?debugFindBaseAddress@hkMemorySystem@@UEAAPEBXPEBXH@Z @ 0x14078e330
    virtual const void* debugFindBaseAddress(const void*, int) { return nullptr; }
};

// hkStackTracer - Stack trace utility
class hkStackTracer {
public:
    static void refreshSymbols(hkStackTracer* instance) {
        // Stub - no implementation needed
    }
};

// hkMemoryInitUtil - Memory initialization utility
// IDA: ?initDefault@hkMemoryInitUtil@@YAPEAVhkMemoryRouter@@PEAVhkMemoryAllocator@@AEBUFrameInfo@hkMemorySystem@@@Z @ 0x1402f7980
struct hkMemoryInitUtil {
    static hkMemorySystem* s_system;
    static void (*s_onQuitFunc)();

    static hkMemoryRouter* initDefault(
        hkMemoryAllocator* alloc,
        const hkMemorySystem::FrameInfo& info) {
        static hkMallocAllocator defaultAllocator;
        if (!alloc) {
            alloc = &defaultAllocator;
        }
        hkMallocAllocator::m_defaultMallocAllocator =
            static_cast<hkMallocAllocator*>(alloc);
        hkMemorySystem* system = new hkMemorySystem();
        s_system = system;
        hkMemorySystem::replaceInstance(system);
        return system->mainInit(
            info,
            hkMemorySystem::Flags(hkMemorySystem::FLAG_ALL));
    }

    // IDA: ?outputDebugString@hkMemoryInitUtil@@YAXPEBDPEAX@Z @ 0x14078f500
    static void outputDebugString(const char* s, void* = nullptr);

    // IDA: ?quit@hkMemoryInitUtil@@YA?AUhkResult@@XZ @ 0x14078f530
    static hkResult quit();

    // IDA: ?refreshDebugSymbols@hkMemoryInitUtil@@YAXXZ @ 0x14078f5b0
    static void refreshDebugSymbols();
};

// hkPackedVector3 - Packed 16-bit vector with shared exponent
// Forward declaration - implementation after hkVector4
class hkPackedVector3;

// hkPackedVector8_3 - Packed 8-bit vector with shared exponent
// Forward declaration - implementation after hkVector4
class hkPackedVector8_3;


// hkMath - Math utilities namespace
namespace hkMath {
    // IDA: hkMath::sqrt @ 0x14078bcd0
    inline float sqrt(float r) {
        return std::sqrt(r);
    }

    // IDA: hkMath::min2_int_ @ 0x14078e180
    inline int min2_int(unsigned int x, unsigned int y) {
        return static_cast<int>(x) < static_cast<int>(y) ? x : y;
    }

    // IDA: hkMath::min2_int__0 @ 0x14078e390
    inline int min2_int_0(unsigned int x, unsigned int y) {
        return static_cast<int>(x) < static_cast<int>(y) ? x : y;
    }

    // IDA: hkMath::max2_int
    inline int max2_int(int x, int y) {
        return x > y ? x : y;
    }

    // IDA: hkMath::max2_int_ @ 0x140796720
    inline int max2_int_(int x, int y) {
        return x > y ? x : y;
    }

    // IDA: hkMath::max2_unsigned___int64_ @ 0x140796700
    inline unsigned long long max2_unsigned_uint64(unsigned long long x, unsigned long long y) {
        return x > y ? x : y;
    }

    // IDA: hkMath::quadFabs @ 0x14078bcf0
    inline __m128 quadFabs(__m128 v) {
#ifdef _WIN32
        // Clear sign bit by AND with complement of sign mask
        return _mm_andnot_ps(_mm_set1_ps(-0.0f), v);
#else
        return v;
#endif
    }

    // IDA: hkMath::quadFabs_0 @ 0x14078f610
    inline __m128 quadFabs_0(__m128 v) {
#ifdef _WIN32
        return _mm_andnot_ps(_mm_set1_ps(-0.0f), v);
#else
        return v;
#endif
    }

    // IDA: hkMath::quadReciprocal @ 0x14078bc80
    // Newton-Raphson refinement of reciprocal approximation
    inline __m128 quadReciprocal(__m128 v) {
#ifdef _WIN32
        __m128 rcp = _mm_rcp_ps(v);
        // One iteration of Newton-Raphson: rcp = rcp * (2 - v * rcp)
        const __m128 two = _mm_set1_ps(2.0f);
        return _mm_mul_ps(_mm_sub_ps(two, _mm_mul_ps(v, rcp)), rcp);
#else
        return _mm_div_ps(_mm_set1_ps(1.0f), v);
#endif
    }

    // IDA: hkMath::quadReciprocalSquareRoot @ 0x14078bca0
    // Newton-Raphson refinement of reciprocal square root approximation
    inline __m128 quadReciprocalSquareRoot(__m128 v) {
#ifdef _WIN32
        __m128 rsqrt = _mm_rsqrt_ps(v);
        // One iteration of Newton-Raphson: rsqrt = rsqrt * (1.5 - 0.5 * v * rsqrt * rsqrt)
        const __m128 half = _mm_set1_ps(0.5f);
        const __m128 three_half = _mm_set1_ps(1.5f);
        return _mm_mul_ps(_mm_sub_ps(three_half, _mm_mul_ps(_mm_mul_ps(half, v), _mm_mul_ps(rsqrt, rsqrt))), rsqrt);
#else
        return _mm_div_ps(_mm_set1_ps(1.0f), _mm_sqrt_ps(v));
#endif
    }

    // IDA: hkMath::quadSinApproximation @ 0x14078bd10
    // Fast sine approximation using polynomial
    __m128 quadSinApproximation(__m128 x);

    // IDA: hkMath::quadSin @ 0x14078bd80
    // Accurate sine using polynomial coefficients
    __m128 quadSin(__m128 x);

    // IDA: hkMath::quadAsin @ 0x14078bea0
    // Arcsine using polynomial approximation
    __m128 quadAsin(__m128 x);

    // IDA: hkMath::quadAcos @ 0x14078bfa0
    // Arccosine using polynomial approximation
    __m128 quadAcos(__m128 x);

    // IDA: hkMath::quadSinCosApproximation @ 0x14078c0b0
    // Fast sine/cosine approximation
    __m128 quadSinCosApproximation(__m128 x);

    // IDA: hkMath::quadSinCos @ 0x14078c120
    // Accurate sine/cosine using polynomial
    __m128 quadSinCos(__m128 x);

    // IDA: hkMath::min2_int__1 @ 0x140792140
    inline int min2_int_1(unsigned int x, unsigned int y) {
        return static_cast<int>(x) < static_cast<int>(y) ? x : y;
    }

    // IDA: hkMath::min2_int__2 @ 0x1407a5b90
    inline int min2_int_2(unsigned int x, unsigned int y) {
        return static_cast<int>(x) < static_cast<int>(y) ? x : y;
    }

    // IDA: hkMath::min2_int__3 @ 0x1407a8840
    inline int min2_int_3(unsigned int x, unsigned int y) {
        return static_cast<int>(x) < static_cast<int>(y) ? x : y;
    }

    // IDA: hkMath::min2_int__4 @ 0x1407aa810
    inline int min2_int_4(unsigned int x, unsigned int y) {
        return static_cast<int>(x) < static_cast<int>(y) ? x : y;
    }

    // IDA: hkMath::max2_int__0 @ 0x1407a5ba0
    inline int max2_int_0(int x, int y) {
        return x > y ? x : y;
    }

    // IDA: hkMath::max2_int__1 @ 0x1407a8f90
    inline int max2_int_1(int x, int y) {
        return x > y ? x : y;
    }

    // IDA: hkMath::max2___int64_ @ 0x1407abe00
    inline long long max2_int64(long long x, long long y) {
        return x > y ? x : y;
    }

    // IDA: hkMath::quadReciprocalSquareRoot_1 @ 0x140799b30
    inline __m128 quadReciprocalSquareRoot_1(__m128 v) {
#ifdef _WIN32
        __m128 rsqrt = _mm_rsqrt_ps(v);
        const __m128 three_half = _mm_set1_ps(3.0f);
        const __m128 half = _mm_set1_ps(0.5f);
        return _mm_mul_ps(_mm_sub_ps(three_half, _mm_mul_ps(_mm_mul_ps(v, rsqrt), rsqrt)), _mm_mul_ps(half, rsqrt));
#else
        return _mm_div_ps(_mm_set1_ps(1.0f), _mm_sqrt_ps(v));
#endif
    }

    // IDA: hkMath::sqrt_0 @ 0x140799b60
    inline float sqrt_0(float r) {
        return std::sqrt(r);
    }

    // IDA: hkMath::sqrt_1 @ 0x1407ad750
    inline float sqrt_1(float r) {
        return std::sqrt(r);
    }

    // IDA: hkMath::quadFabs_1 @ 0x140799b80
    inline __m128i quadFabs_1(__m128i v) {
#ifdef _WIN32
        return _mm_srli_epi32(_mm_slli_epi32(v, 1), 1);
#else
        return v;
#endif
    }

    // IDA: hkMath::quadFabs_2 @ 0x14079a000
    inline __m128 quadFabs_2(__m128 v) {
#ifdef _WIN32
        return _mm_andnot_ps(_mm_set1_ps(-0.0f), v);
#else
        return v;
#endif
    }

    // IDA: hkMath::quadFabs_3 @ 0x14079a5a0
    inline __m128 quadFabs_3(__m128 v) {
#ifdef _WIN32
        return _mm_andnot_ps(_mm_set1_ps(-0.0f), v);
#else
        return v;
#endif
    }

    // IDA: hkMath::quadFabs_4 @ 0x1407ad7a0
    inline __m128 quadFabs_4(__m128 v) {
#ifdef _WIN32
        return _mm_andnot_ps(_mm_set1_ps(-0.0f), v);
#else
        return v;
#endif
    }

    // IDA: hkMath::quadFabs_5 @ 0x1407af2d0
    inline __m128 quadFabs_5(__m128 v) {
#ifdef _WIN32
        return _mm_andnot_ps(_mm_set1_ps(-0.0f), v);
#else
        return v;
#endif
    }

    // IDA: hkMath::quadFabs_6 @ 0x1407b2cf0
    inline __m128 quadFabs_6(__m128 v) {
#ifdef _WIN32
        return _mm_andnot_ps(_mm_set1_ps(-0.0f), v);
#else
        return v;
#endif
    }

    // IDA: hkMath::quadReciprocal_1 @ 0x140799fb0
    inline __m128 quadReciprocal_1(__m128 v) {
#ifdef _WIN32
        __m128 rcp = _mm_rcp_ps(v);
        const __m128 two = _mm_set1_ps(2.0f);
        return _mm_mul_ps(_mm_sub_ps(two, _mm_mul_ps(v, rcp)), rcp);
#else
        return _mm_div_ps(_mm_set1_ps(1.0f), v);
#endif
    }

    // IDA: hkMath::quadReciprocal_2 @ 0x1407ad700
    inline __m128 quadReciprocal_2(__m128 v) {
#ifdef _WIN32
        __m128 rcp = _mm_rcp_ps(v);
        const __m128 two = _mm_set1_ps(2.0f);
        return _mm_mul_ps(_mm_sub_ps(two, _mm_mul_ps(v, rcp)), rcp);
#else
        return _mm_div_ps(_mm_set1_ps(1.0f), v);
#endif
    }

    // IDA: hkMath::quadReciprocal_3 @ 0x1407af280
    inline __m128 quadReciprocal_3(__m128 v) {
#ifdef _WIN32
        __m128 rcp = _mm_rcp_ps(v);
        const __m128 two = _mm_set1_ps(2.0f);
        return _mm_mul_ps(_mm_sub_ps(two, _mm_mul_ps(v, rcp)), rcp);
#else
        return _mm_div_ps(_mm_set1_ps(1.0f), v);
#endif
    }

    // IDA: hkMath::quadReciprocal_4 @ 0x1407b26d0
    inline __m128 quadReciprocal_4(__m128 v) {
#ifdef _WIN32
        __m128 rcp = _mm_rcp_ps(v);
        const __m128 two = _mm_set1_ps(2.0f);
        return _mm_mul_ps(_mm_sub_ps(two, _mm_mul_ps(v, rcp)), rcp);
#else
        return _mm_div_ps(_mm_set1_ps(1.0f), v);
#endif
    }

    // IDA: hkMath::quadReciprocal_5 @ 0x1407b2cd0
    inline __m128 quadReciprocal_5(__m128 v) {
#ifdef _WIN32
        __m128 rcp = _mm_rcp_ps(v);
        const __m128 two = _mm_set1_ps(2.0f);
        return _mm_mul_ps(_mm_sub_ps(two, _mm_mul_ps(v, rcp)), rcp);
#else
        return _mm_div_ps(_mm_set1_ps(1.0f), v);
#endif
    }

    // IDA: hkMath::quadReciprocalSquareRoot_2 @ 0x140799fd0
    inline __m128 quadReciprocalSquareRoot_2(__m128 v) {
#ifdef _WIN32
        __m128 rsqrt = _mm_rsqrt_ps(v);
        const __m128 three_half = _mm_set1_ps(3.0f);
        const __m128 half = _mm_set1_ps(0.5f);
        return _mm_mul_ps(_mm_sub_ps(three_half, _mm_mul_ps(_mm_mul_ps(v, rsqrt), rsqrt)), _mm_mul_ps(half, rsqrt));
#else
        return _mm_div_ps(_mm_set1_ps(1.0f), _mm_sqrt_ps(v));
#endif
    }

    // IDA: hkMath::quadReciprocalSquareRoot_3 @ 0x14079d570
    inline __m128 quadReciprocalSquareRoot_3(__m128 v) {
#ifdef _WIN32
        __m128 rsqrt = _mm_rsqrt_ps(v);
        const __m128 three_half = _mm_set1_ps(3.0f);
        const __m128 half = _mm_set1_ps(0.5f);
        return _mm_mul_ps(_mm_sub_ps(three_half, _mm_mul_ps(_mm_mul_ps(v, rsqrt), rsqrt)), _mm_mul_ps(half, rsqrt));
#else
        return _mm_div_ps(_mm_set1_ps(1.0f), _mm_sqrt_ps(v));
#endif
    }

    // IDA: hkMath::quadReciprocalSquareRoot_4 @ 0x1407ad720
    inline __m128 quadReciprocalSquareRoot_4(__m128 v) {
#ifdef _WIN32
        __m128 rsqrt = _mm_rsqrt_ps(v);
        const __m128 three_half = _mm_set1_ps(3.0f);
        const __m128 half = _mm_set1_ps(0.5f);
        return _mm_mul_ps(_mm_sub_ps(three_half, _mm_mul_ps(_mm_mul_ps(v, rsqrt), rsqrt)), _mm_mul_ps(half, rsqrt));
#else
        return _mm_div_ps(_mm_set1_ps(1.0f), _mm_sqrt_ps(v));
#endif
    }

    // IDA: hkMath::quadReciprocalSquareRoot_5 @ 0x1407af2a0
    inline __m128 quadReciprocalSquareRoot_5(__m128 v) {
#ifdef _WIN32
        __m128 rsqrt = _mm_rsqrt_ps(v);
        const __m128 three_half = _mm_set1_ps(3.0f);
        const __m128 half = _mm_set1_ps(0.5f);
        return _mm_mul_ps(_mm_sub_ps(three_half, _mm_mul_ps(_mm_mul_ps(v, rsqrt), rsqrt)), _mm_mul_ps(half, rsqrt));
#else
        return _mm_div_ps(_mm_set1_ps(1.0f), _mm_sqrt_ps(v));
#endif
    }

    // IDA: hkMath::sqrtInverse @ 0x1407ad770
    inline float sqrtInverse(float r) {
        return 1.0f / std::sqrt(r);
    }

    // IDA: hkMath::fabs @ 0x1407ad7c0 - absolute value via bit manipulation
    // This uses the trick: (2 * float_bits) >> 1 clears the sign bit
    // Template instantiations: fabs_0 (0x1407ed950), fabs_1 (0x140825460), fabs_2 (0x140829510)
    inline float fabs(float r) {
        uint32_t bits;
        memcpy(&bits, &r, sizeof(bits));
        bits &= 0x7FFFFFFF;  // Clear sign bit
        float result;
        memcpy(&result, &bits, sizeof(result));
        return result;
    }

    // IDA: hkMath::interpolate2d @ 0x1408073f0
    // Bilinear interpolation
    inline float interpolate2d(float x, float y, float v00, float v10, float v01, float v11) {
        float tx = x - std::floor(x);
        float ty = y - std::floor(y);
        float a = v00 * (1.0f - tx) + v10 * tx;
        float b = v01 * (1.0f - tx) + v11 * tx;
        return a * (1.0f - ty) + b * ty;
    }

    // IDA: hkMath::atan2Approximation @ 0x140829530
    // Fast approximation of atan2
    inline float atan2Approximation(float y, float x) {
        // Use standard atan2 for accuracy
        return std::atan2(y, x);
    }
}

// Forward declarations for hkString namespace
class hkContainerHeapAllocator;
template<typename T, typename Allocator> class hkArray;

// hkString - String utilities namespace
// IDA: Multiple functions at 0x14079bxxx
namespace hkString {
    // IDA: ?toUpper@hkString@@YADD@Z @ 0x14079b2c0
    inline char toUpper(char c) {
        if (static_cast<unsigned char>(c - 'a') <= 25) {
            return c - 32;
        }
        return c;
    }

    // IDA: ?toLower@hkString@@YADD@Z @ 0x14079b2e0
    inline char toLower(char c) {
        if (static_cast<unsigned char>(c - 'A') <= 25) {
            return c + 32;
        }
        return c;
    }

    // IDA: ?strCmp@hkString@@YAHPEBD0@Z @ 0x14079b370
    inline int strCmp(const char* a, const char* b) {
        return strcmp(a, b);
    }

    // IDA: ?strNcmp@hkString@@YAHPEBD0H@Z @ 0x14079b3a0
    inline int strNcmp(const char* a, const char* b, unsigned int n) {
        return strncmp(a, b, n);
    }

    // IDA: ?strCasecmp@hkString@@YAHPEBD0@Z @ 0x14079b3b0
    inline int strCasecmp(const char* a, const char* b) {
#ifdef _WIN32
        return _stricmp(a, b);
#else
        return strcasecmp(a, b);
#endif
    }

    // IDA: ?strNcasecmp@hkString@@YAHPEBD0H@Z @ 0x14079b410
    inline int strNcasecmp(const char* a, const char* b, unsigned int n) {
#ifdef _WIN32
        return _strnicmp(a, b, n);
#else
        return strncasecmp(a, b, n);
#endif
    }

    // IDA: ?strCpy@hkString@@YAXPEADPEBD@Z @ 0x14079b480
    inline void strCpy(char* dst, const char* src) {
        strcpy(dst, src);
    }

    // IDA: ?strNcpy@hkString@@YAXPEADPEBDH@Z @ 0x14079b4b0
    inline void strNcpy(char* dst, const char* src, unsigned int n) {
        if (n) {
            strncpy(dst, src, n);
        }
    }

    // IDA: ?strLen@hkString@@YAHPEBD@Z @ 0x14079b4d0
    inline int strLen(const char* src) {
        return static_cast<int>(strlen(src));
    }

    // IDA: ?strCat@hkString@@YAXPEADPEBD@Z @ 0x14079b500
    inline void strCat(char* dst, const char* src) {
        strcat(dst, src);
    }

    // IDA: ?strNcat@hkString@@YAXPEADPEBDH@Z @ 0x14079b540
    inline void strNcat(char* dst, const char* src, unsigned int n) {
        strncat(dst, src, n);
    }

    // IDA: ?atoi@hkString@@YAHPEBDH@Z @ 0x14079b560
    inline int atoi(const char* in, int base) {
        return static_cast<int>(strtoul(in, nullptr, base));
    }

    // IDA: ?atoll@hkString@@YA_JPEBDH@Z @ 0x14079b580
    inline long long atoll(const char* in, int base) {
#ifdef _WIN32
        return _strtoi64(in, nullptr, base);
#else
        return strtoll(in, nullptr, base);
#endif
    }

    // IDA: ?vsnprintf@hkString@@YAHPEADHPEBDPEAX@Z @ 0x14079b300
    inline int vsnprintf(char* buf, int n, const char* fmt, va_list args) {
        return ::vsnprintf(buf, n, fmt, args);
    }

    // IDA: ?snprintf@hkString@@YAHPEADHPEBDZZ @ 0x14079b310
    inline int snprintf(char* buf, int n, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        int result = ::vsnprintf(buf, n, fmt, args);
        va_end(args);
        return result;
    }

    // IDA: ?sprintf@hkString@@YAHPEADPEBDZZ @ 0x14079b340
    inline int sprintf(char* buf, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        int result = ::vsprintf(buf, fmt, args);
        va_end(args);
        return result;
    }

    // IDA: ?atoull@hkString@@YA_KPEBDH@Z @ 0x14079b5a0
    inline unsigned long long atoull(const char* in, int base) {
#ifdef _WIN32
        return _strtoui64(in, nullptr, base);
#else
        return strtoull(in, nullptr, base);
#endif
    }

    // IDA: ?atof@hkString@@YAMPEBD@Z @ 0x14079b5c0
    inline float atof(const char* in) {
        return static_cast<float>(strtod(in, nullptr));
    }

    // IDA: ?strStr@hkString@@YAPEBDPEBD0@Z @ 0x14079b5e0
    inline const char* strStr(const char* str, const char* substr) {
        return strstr(str, substr);
    }

    // IDA: ?strChr@hkString@@YAPEBDPEBDH@Z @ 0x14079b5f0
    inline const char* strChr(const char* str, int c) {
        return strchr(str, c);
    }

    // IDA: ?strRchr@hkString@@YAPEBDPEBDH@Z @ 0x14079b600
    inline const char* strRchr(const char* str, int c) {
        return strrchr(str, c);
    }

    // IDA: ?strDup@hkString@@YAPEADPEBDAEAVhkMemoryAllocator@@@Z @ 0x14079b610
    inline char* strDup(const char* src, hkMemoryAllocator& alloc) {
        int len = strLen(src);
        char* result = static_cast<char*>(hkMemoryRouter::easyAlloc(alloc, len + 1));
        strCpy(result, src);
        return result;
    }

    // IDA: ?strFree@hkString@@YAXPEADAEAVhkMemoryAllocator@@@Z @ 0x14079b650
    inline void strFree(char* s, hkMemoryAllocator& alloc) {
        hkMemoryRouter::easyFree(alloc, s);
    }

    // IDA: ?strNdup@hkString@@YAPEADPEBDHAEAVhkMemoryAllocator@@@Z @ 0x14079b670
    inline char* strNdup(const char* src, int maxlen, hkMemoryAllocator& alloc) {
        int len = strLen(src);
        if (len > maxlen) len = maxlen;
        char* result = static_cast<char*>(hkMemoryRouter::easyAlloc(alloc, len + 1));
        strNcpy(result, src, len);
        result[len] = '\0';
        return result;
    }

    // IDA: ?strLwr@hkString@@YAPEADPEAD@Z @ 0x14079b6d0
    inline char* strLwr(char* s) {
        char* p = s;
        if (*s) {
            do {
                *p = toLower(*p);
                p++;
            } while (*p);
        }
        return s;
    }

    // IDA: ?strUpr@hkString@@YAPEADPEAD@Z @ 0x14079b710
    inline char* strUpr(char* s) {
        char* p = s;
        if (*s) {
            do {
                *p = toUpper(*p);
                p++;
            } while (*p);
        }
        return s;
    }

    // IDA: ?memCpy@hkString@@YAXPEAXPEBXH@Z @ 0x14079b750
    inline void memCpy(void* dst, const void* src, unsigned int n) {
        memcpy(dst, src, n);
    }

    // IDA: ?memMove@hkString@@YAXPEAXPEBXH@Z @ 0x14079b760
    inline void memMove(void* dst, const void* src, unsigned int n) {
        memmove(dst, src, n);
    }

    // IDA: ?memSet@hkString@@YAXPEAXHH@Z @ 0x14079b770
    inline void memSet(void* dst, int val, unsigned int n) {
        memset(dst, val, n);
    }

    // IDA: ?memCmp@hkString@@YAHPEBX0H@Z @ 0x14079b780
    inline int memCmp(const void* buf1, const void* buf2, unsigned int n) {
        return memcmp(buf1, buf2, n);
    }

    // IDA: ?beginsWith@hkString@@YA?AVhkBool@@PEBD0@Z @ 0x14079b790
    inline hkBool beginsWith(const char* a, const char* b) {
        hkBool result;
        if (*b) {
            while (*a == *b) {
                b++;
                if (!*b) {
                    result.m_bool = true;
                    return result;
                }
                a++;
            }
            result.m_bool = false;
        } else {
            result.m_bool = true;
        }
        return result;
    }

    // IDA: ?endsWith@hkString@@YA?AVhkBool@@PEBD0@Z @ 0x14079b7d0
    inline hkBool endsWith(const char* a, const char* b) {
        hkBool result;
        int aLen = strLen(a);
        int bLen = strLen(b);

        if (aLen < bLen) {
            result.m_bool = false;
            return result;
        }

        int start = aLen - bLen;
        for (int i = 0; i < bLen; i++) {
            if (a[start + i] != b[i]) {
                result.m_bool = false;
                return result;
            }
        }
        result.m_bool = true;
        return result;
    }

    // IDA: ?lastIndexOf@hkString@@YAHPEBDD@Z @ 0x14079b850
    inline int lastIndexOf(const char* str, char c) {
        const char* found = strRchr(str, c);
        if (found) {
            return static_cast<int>(found - str);
        }
        return -1;
    }

    // IDA: ?indexOf@hkString@@YAHPEBDDHH@Z @ 0x14079b880
    inline int indexOf(const char* str, char c, int startIndex = 0, int endIndex = 0x7FFFFFFF) {
        if (startIndex > 0) {
            // Skip to start index
            for (int i = 0; i < startIndex && str[i]; i++) {}
        }

        for (int i = startIndex; i < endIndex && str[i]; i++) {
            if (str[i] == c) {
                return i;
            }
        }
        return -1;
    }

    // IDA: ?memClear128@hkString@@YAXPEAXH@Z @ 0x14079b8f0
    inline void memClear128(void* dst, int numBytes) {
        int numBlocks = numBytes >> 4;
        __m128i* p = static_cast<__m128i*>(dst);
        for (int i = 0; i < numBlocks; i++) {
            p[i] = _mm_setzero_si128();
        }
    }

    // IDA: ?strFree@hkString@@YAXPEAD@Z @ 0x14079b920
    // Uses TLS to get memory router
    inline void strFree(char* s);

    // IDA: ?strDup@hkString@@YAPEADPEBD@Z @ 0x14079b950
    // Uses TLS to get memory router
    inline char* strDup(const char* src);

    // IDA: ?strNdup@hkString@@YAPEADPEBDH@Z @ 0x14079b980
    // Uses TLS to get memory router
    inline char* strNdup(const char* src, int maxlen);

    // findAllOccurrences defined later after hkArray is fully defined
}

// hkHardwareInfo - Hardware information structure
struct hkHardwareInfo {
    int m_numThreads;
    int m_numCores;
    // Additional members...
};

// IDA: ?hkGetHardwareInfo@@YAXAEAUhkHardwareInfo@@@Z @ 0x14078e3a0
inline void hkGetHardwareInfo(hkHardwareInfo& info) {
#ifdef _WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    info.m_numThreads = sysInfo.dwNumberOfProcessors;
    info.m_numCores = sysInfo.dwNumberOfProcessors;
#else
    info.m_numThreads = 1;
    info.m_numCores = 1;
#endif
}

// hkCriticalSection - Critical section wrapper
// IDA: ??0hkCriticalSection@@QEAA@H@Z @ 0x140792fb0
class hkCriticalSection {
public:
#ifdef _WIN32
    CRITICAL_SECTION m_section;
#else
    void* m_section;  // pthread_mutex_t placeholder
#endif

    // IDA: ??0hkCriticalSection@@QEAA@H@Z @ 0x140792fb0
    hkCriticalSection(DWORD spinCount = 0) {
#ifdef _WIN32
        if (!spinCount) {
            hkHardwareInfo hwInfo;
            hkGetHardwareInfo(hwInfo);
            spinCount = 1000 * hwInfo.m_numThreads;
        }
        InitializeCriticalSectionAndSpinCount(&m_section, spinCount);
#endif
    }

    ~hkCriticalSection() {
#ifdef _WIN32
        DeleteCriticalSection(&m_section);
#endif
    }

    void enter() {
#ifdef _WIN32
        EnterCriticalSection(&m_section);
#endif
    }

    void leave() {
#ifdef _WIN32
        LeaveCriticalSection(&m_section);
#endif
    }
};

// ============================================================================
// Havok Math Types
// ============================================================================

// hkSimdReal - Havok SIMD real number
class hkSimdReal {
public:
#ifdef _WIN32
    __m128 m_real;  // SSE register for SIMD operations
#else
    float m_value;
#endif

    hkSimdReal() {
#ifdef _WIN32
        m_real = _mm_setzero_ps();
#else
        m_value = 0.0f;
#endif
    }

    hkSimdReal(float v) {
#ifdef _WIN32
        m_real = _mm_set1_ps(v);
#else
        m_value = v;
#endif
    }

    // IDA: ??0hkSimdReal@@QEAA@AEBM@Z @ 0x1402f7800
    explicit hkSimdReal(const float* x) {
#ifdef _WIN32
        m_real = _mm_shuffle_ps(_mm_load_ss(x), _mm_load_ss(x), 0);
#else
        m_value = *x;
#endif
    }

    // IDA: ?setFromFloat@hkSimdReal@@QEAAXAEBM@Z @ 0x140277910
    void setFromFloat(const float* x) {
#ifdef _WIN32
        m_real = _mm_shuffle_ps(_mm_load_ss(x), _mm_load_ss(x), 0);
#else
        m_value = *x;
#endif
    }

    // IDA: ?fromFloat@hkSimdReal@@SA?AV1@AEBM@Z @ 0x140277960
    static hkSimdReal fromFloat(const float* x) {
        hkSimdReal r;
        r.setFromFloat(x);
        return r;
    }

    // Compatibility method for existing code
    static hkSimdReal* fromFloat(hkSimdReal* result, float* val) {
        result->setFromFloat(val);
        return result;
    }
};

// Forward declaration for hkQuaternion (needed by hkVector4::setRotatedDir)
struct hkQuaternion;

// hkVector4 - Havok 4D vector
class hkVector4 {
public:
#ifdef _WIN32
    union {
        __m128 m_quad;
        struct { float x, y, z, w; };
    };
#else
    float x, y, z, w;
#endif

    hkVector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    hkVector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

    // IDA: ??0hkVector4@@QEAA@MMMM@Z @ 0x1401ade50
#ifdef _WIN32
    hkVector4(float a, float b, float c, unsigned int d) {
        m_quad = _mm_unpacklo_ps(
            _mm_unpacklo_ps(_mm_load_ss(&a), _mm_load_ss(&c)),
            _mm_unpacklo_ps(_mm_load_ss(&b), _mm_load_ss(reinterpret_cast<float*>(&d))));
    }
#endif

    // IDA: ??0hkVector4@@QEAA@AEBV0@@Z @ 0x1401adee0
    hkVector4(const hkVector4& v) {
#ifdef _WIN32
        m_quad = v.m_quad;
#else
        x = v.x; y = v.y; z = v.z; w = v.w;
#endif
    }

    // IDA: ?set@hkVector4@@QEAAXMMMM@Z @ 0x1401adf00
    void set(float a, float b, float c, unsigned int d) {
#ifdef _WIN32
        m_quad = _mm_unpacklo_ps(
            _mm_unpacklo_ps(_mm_load_ss(&a), _mm_load_ss(&c)),
            _mm_unpacklo_ps(_mm_load_ss(&b), _mm_load_ss(reinterpret_cast<float*>(&d))));
#else
        x = a; y = b; z = c; w = *reinterpret_cast<float*>(&d);
#endif
    }

    // IDA: ??$getConstant@$0CO@@hkVector4@@SAAEBV0@XZ @ 0x140275fe0
    // Template getConstant<N> - returns pointer to global constant vector
    template<int N>
    static const hkVector4* getConstant() {
        return reinterpret_cast<const hkVector4*>(&g_vectorConstants[N * 4]);
    }

    // IDA: ?getZero@hkVector4@@SAAEBV1@XZ @ 0x1402f8de0
    static const hkVector4* getZero() {
        return getConstant<1>();  // Constant 1 is zero vector
    }

    // IDA: ??$load@$02$00@hkVector4@@QEAAXPEBM@Z @ 0x140277650
    // load<3,1> - load 3 floats from memory into vector
    template<int a, int b>
    static void load(hkVector4* v, const float* p) {
#ifdef _WIN32
        // IDA: unroll_load<3,0>::apply uses _mm_movelh_ps
        v->m_quad = _mm_movelh_ps(
            _mm_load_sd(reinterpret_cast<const double*>(p)),
            _mm_load_ss(p + 2));
#else
        v->x = p[0]; v->y = p[1]; v->z = p[2]; v->w = 0.0f;
#endif
    }

    // IDA: ??$store@$02$00@hkVector4@@QEBAXPEAM@Z @ 0x1402774b0
    // store<3,1> - store 3 floats to memory from vector
    template<int a, int b>
    static void store(const hkVector4* v, float* p) {
#ifdef _WIN32
        // IDA: unroll_store<3,0>::apply - stores low 8 bytes + high 4 bytes
        _mm_storel_pi(reinterpret_cast<__m64*>(p), v->m_quad);
        __m128 high = _mm_movehl_ps(v->m_quad, v->m_quad);
        _mm_store_ss(p + 2, high);
#else
        p[0] = v->x; p[1] = v->y; p[2] = v->z;
#endif
    }

    // IDA: ??$zeroComponent@$02@hkVector4@@QEAAXXZ @ 0x140277680
    // zeroComponent<3> - zero the 4th component (w)
    template<int a>
    static void zeroComponent(hkVector4* v) {
#ifdef _WIN32
        // IDA: shift left 4 bytes, then shift right 4 bytes (clears w)
        __m128i shifted = _mm_slli_si128(_mm_castps_si128(v->m_quad), 4);
        v->m_quad = _mm_castsi128_ps(_mm_srli_si128(shifted, 4));
#else
        v->w = 0.0f;
#endif
    }

    // IDA: ?mul@hkVector4@@QEAAXAEBVhkSimdReal@@@Z @ 0x1402778e0
    static void mul(hkVector4* v, const hkSimdReal* s) {
#ifdef _WIN32
        v->m_quad = _mm_mul_ps(s->m_real, v->m_quad);
#else
        v->x *= s->m_value;
        v->y *= s->m_value;
        v->z *= s->m_value;
        v->w *= s->m_value;
#endif
    }

    // IDA: ?setMul@hkVector4@@QEAAXAEBV1@AEBVhkSimdReal@@@Z @ 0x1402778a0
    static void setMul(hkVector4* result, const hkVector4* v1, const hkSimdReal* r) {
#ifdef _WIN32
        result->m_quad = _mm_mul_ps(r->m_real, v1->m_quad);
#else
        result->x = v1->x * r->m_value;
        result->y = v1->y * r->m_value;
        result->z = v1->z * r->m_value;
        result->w = v1->w * r->m_value;
#endif
    }

    // IDA: ?setAll@hkVector4@@QEAAXAEBVhkSimdReal@@@Z @ 0x1402f8e90
    static void setAll(hkVector4* v, const hkSimdReal* s) {
#ifdef _WIN32
        v->m_quad = s->m_real;
#else
        v->x = s->m_value;
        v->y = s->m_value;
        v->z = s->m_value;
        v->w = s->m_value;
#endif
    }

    // IDA: ?setMul@hkVector4@@QEAAXAEBV1@0@Z @ 0x1402776e0
    void setMul(const hkVector4* v0, const hkVector4* v1) {
#ifdef _WIN32
        m_quad = _mm_mul_ps(v0->m_quad, v1->m_quad);
#else
        x = v0->x * v1->x;
        y = v0->y * v1->y;
        z = v0->z * v1->z;
        w = v0->w * v1->w;
#endif
    }

    // IDA: ?setAdd@hkVector4@@QEAAXAEBV1@0@Z @ 0x1402f78d0
    static void setAdd(hkVector4* result, const hkVector4* v0, const hkVector4* v1) {
#ifdef _WIN32
        result->m_quad = _mm_add_ps(v0->m_quad, v1->m_quad);
#else
        result->x = v0->x + v1->x;
        result->y = v0->y + v1->y;
        result->z = v0->z + v1->z;
        result->w = v0->w + v1->w;
#endif
    }

    // IDA: ?add@hkVector4@@QEAAXAEBV1@@Z @ 0x1402f78a0
    static void add(hkVector4* v, const hkVector4* a) {
        setAdd(v, v, a);
    }

    // IDA: ?add4@hkVector4@@QEAAXAEBV1@@Z @ 0x1402f8a10
    void add4(const hkVector4* v) {
        add(this, v);
    }

    // IDA: ?mul4@hkVector4@@QEAAXAEBVhkSimdReal@@@Z @ 0x1402f7840
    static void mul4(hkVector4* v, const hkSimdReal* a) {
        mul(v, a);
    }

    // IDA: ?setSub@hkVector4@@QEAAXAEBV1@0@Z @ 0x140280ef0
    static void setSub(hkVector4* result, const hkVector4* v0, const hkVector4* v1) {
#ifdef _WIN32
        result->m_quad = _mm_sub_ps(v0->m_quad, v1->m_quad);
#else
        result->x = v0->x - v1->x;
        result->y = v0->y - v1->y;
        result->z = v0->z - v1->z;
        result->w = v0->w - v1->w;
#endif
    }

    // IDA: ?setAddMul@hkVector4@@QEAAXAEBV1@0AEBVhkSimdReal@@@Z @ 0x140280f30
    static void setAddMul(hkVector4* result, const hkVector4* a, const hkVector4* b, const hkSimdReal* r) {
#ifdef _WIN32
        result->m_quad = _mm_add_ps(a->m_quad, _mm_mul_ps(r->m_real, b->m_quad));
#else
        result->x = a->x + r->m_value * b->x;
        result->y = a->y + r->m_value * b->y;
        result->z = a->z + r->m_value * b->z;
        result->w = a->w + r->m_value * b->w;
#endif
    }

    // IDA: ?setInterpolate@hkVector4@@QEAAXAEBV1@0AEBVhkSimdReal@@@Z @ 0x140280f80
    static void setInterpolate(hkVector4* result, const hkVector4* v0, const hkVector4* v1, const hkSimdReal* t) {
        hkVector4 d;
        setSub(&d, v1, v0);
        setAddMul(result, v0, &d, t);
    }

    // IDA: ?setInterpolate4@hkVector4@@QEAAXAEBV1@0AEBVhkSimdReal@@@Z @ 0x140280e10
    static void setInterpolate4(hkVector4* result, const hkVector4* a, const hkVector4* b, const hkSimdReal* t) {
        setInterpolate(result, a, b, t);
    }

    // IDA: ?setTransformedPos@hkVector4@@SAXAEAV1@AEBVhkTransform@@AEBV1@@Z
    // Transform position by hkTransform - implemented in HavokTypes.cpp
    static void setTransformedPos(hkVector4* result, const hkTransform* t, const hkVector4* pos);

    // IDA: ?setTransformedInversePos@hkVector4@@SAXAEAV1@AEBVhkTransform@@AEBV1@@Z
    // Transform position by inverse of hkTransform - implemented in HavokTypes.cpp
    static void setTransformedInversePos(hkVector4* result, const hkTransform* t, const hkVector4* pos);

    // IDA: ?setRotatedDir@hkVector4@@QEAAXAEBVhkQuaternion@@AEBV1@@Z @ 0x1407b2aa0
    // Rotate direction vector by quaternion - implemented in HavokTypes.cpp
    void setRotatedDir(const hkQuaternion& quat, const hkVector4& direction);

    // IDA: ?setRotatedInverseDir@hkVector4@@QEAAXAEBVhkQuaternion@@AEBV1@@Z @ 0x1407b2b30
    // Rotate direction vector by inverse quaternion - implemented in HavokTypes.cpp
    void setRotatedInverseDir(const hkQuaternion& quat, const hkVector4& direction);

    // IDA: ?setRotatedDir@hkVector4@@QEAAXAEBVhkMatrix3@@AEBV1@@Z @ 0x1407b26f0
    // Rotate direction vector by matrix - implemented in HavokTypes.cpp
    void setRotatedDir(const hkMatrix3& matrix, const hkVector4& direction);

    // IDA: ?setRotatedInverseDir@hkVector4@@QEAAXAEBVhkMatrix3@@AEBV1@@Z @ 0x1407b2730
    // Rotate direction vector by inverse matrix - implemented in HavokTypes.cpp
    void setRotatedInverseDir(const hkMatrix3& matrix, const hkVector4& direction);

    // IDA: ?setTransformedPos@hkVector4@@QEAAXAEBVhkQsTransform@@AEBV1@@Z @ 0x1407b2830
    // Transform position by hkQsTransform - implemented in HavokTypes.cpp
    void setTransformedPos(const hkQsTransform& t, const hkVector4& pos);

    // IDA: ?setTransformedInversePos@hkVector4@@QEAAXAEBVhkQsTransform@@AEBV1@@Z @ 0x1407b28d0
    // Transform position by inverse hkQsTransform - implemented in HavokTypes.cpp
    void setTransformedInversePos(const hkQsTransform& t, const hkVector4& pos);

    // IDA: ?setTransformedPos@hkVector4@@QEAAXAEBVhkQTransform@@AEBV1@@Z @ 0x1407b2980
    // Transform position by hkQTransform - implemented in HavokTypes.cpp
    void setTransformedPos(const hkQTransform& t, const hkVector4& pos);

    // IDA: ?setTransformedInversePos@hkVector4@@QEAAXAEBVhkQTransform@@AEBV1@@Z @ 0x1407b2a10
    // Transform position by inverse hkQTransform - implemented in HavokTypes.cpp
    void setTransformedInversePos(const hkQTransform& t, const hkVector4& pos);

    // IDA: ?setZero@hkVector4@@QEAAXXZ @ 0x1402f8e60
    void setZero() {
#ifdef _WIN32
        m_quad = _mm_setzero_ps();
#else
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
#endif
    }

    // IDA: ??$setNeg@$03@hkVector4@@QEAAXAEBV0@@Z @ 0x1402f8df0
    template<int N>
    void setNeg(const hkVector4* v) {
#ifdef _WIN32
        m_quad = _mm_xor_ps(v->m_quad, _mask__NegFloat_);
#else
        x = -v->x;
        y = -v->y;
        z = -v->z;
        w = -v->w;
#endif
    }

    // IDA: ??RhkVector4@@QEAAAEAMH@Z @ 0x140280ed0
    float& operator()(int i) { return (&x)[i]; }
    const float& operator()(int i) const { return (&x)[i]; }
};

// hkEnum - Havok enum wrapper
// IDA: ??0?$hkEnum@W4NearbyBoundariesSearchType@AvoidanceProperties@hkaiAvoidanceSolver@@E@@QEAA@W4NearbyBoundariesSearchType@AvoidanceProperties@hkaiAvoidanceSolver@@@Z @ 0x140276e20
template<typename ENUM, typename STORAGE = unsigned char>
class hkEnum {
public:
    STORAGE m_storage;

    hkEnum() : m_storage(0) {}

    // IDA: Constructor from enum value
    hkEnum(ENUM e) : m_storage(static_cast<STORAGE>(e)) {}

    // Implicit conversion to enum
    operator ENUM() const { return static_cast<ENUM>(m_storage); }

    // Assignment from enum
    hkEnum& operator=(ENUM e) {
        m_storage = static_cast<STORAGE>(e);
        return *this;
    }
};

// hkIntVector - Integer vector for SIMD operations
// IDA: uses __m128i for 4 x int32
class hkIntVector {
public:
#ifdef _WIN32
    __m128i m_quad;
#else
    int32_t m_data[4];
#endif

    hkIntVector() {
#ifdef _WIN32
        m_quad = _mm_setzero_si128();
#else
        memset(m_data, 0, sizeof(m_data));
#endif
    }

    // IDA: ?setConvertF32toS32@hkIntVector@@QEAAXAEBVhkVector4@@@Z @ 0x140275ff0
    void setConvertF32toS32(const hkVector4* vIn) {
#ifdef _WIN32
        __m128 v = vIn->m_quad;
        __m128 two31 = _mm_set1_ps(2147483648.0f);  // 2^31
        __m128 overflow = _mm_cmple_ps(two31, v);
        __m128i converted = _mm_cvttps_epi32(v);
        m_quad = _mm_xor_si128(converted, _mm_castps_si128(overflow));
#else
        m_data[0] = static_cast<int>(vIn->x);
        m_data[1] = static_cast<int>(vIn->y);
        m_data[2] = static_cast<int>(vIn->z);
        m_data[3] = static_cast<int>(vIn->w);
#endif
    }

    // IDA: ?setAddU32@hkIntVector@@QEAAXAEBV1@0@Z @ 0x140276080
    void setAddU32(const hkIntVector* a, const hkIntVector* b) {
#ifdef _WIN32
        m_quad = _mm_add_epi32(a->m_quad, b->m_quad);
#else
        m_data[0] = a->m_data[0] + b->m_data[0];
        m_data[1] = a->m_data[1] + b->m_data[1];
        m_data[2] = a->m_data[2] + b->m_data[2];
        m_data[3] = a->m_data[3] + b->m_data[3];
#endif
    }

    // IDA: ??$getU16@$00@hkIntVector@@QEBAGXZ @ 0x1402760c0 (element 1)
    // IDA: ??$getU16@$02@hkIntVector@@QEBAGXZ @ 0x1402760e0 (element 3)
    // IDA: ??$getU16@$04@hkIntVector@@QEBAGXZ @ 0x140276100 (element 5)
    // IDA: ??$getU16@$06@hkIntVector@@QEBAGXZ @ 0x140276120 (element 7)
    template<int N>
    uint16_t getU16() const {
#ifdef _WIN32
        return static_cast<uint16_t>(_mm_extract_epi16(m_quad, N));
#else
        return static_cast<uint16_t>(m_data[N / 2] >> (16 * (N % 2)));
#endif
    }
};

// hkPackedUnitVector - Packed unit vector (3 elements packed into 16-bit values)
// IDA: size = 6 bytes (3 x uint16_t)
template<int N>
class hkPackedUnitVector {
public:
    uint16_t m_vec[3];

    hkPackedUnitVector() {
        m_vec[0] = 0;
        m_vec[1] = 0;
        m_vec[2] = 0;
    }

    // IDA: ?set@?$hkPackedUnitVector@$02@@QEAAXMMMM@Z @ 0x14026c480
    // Instance method - sets this vector
    void set(float x, float y, float z, unsigned int w) {
        hkVector4 v;
        v.set(x, y, z, w);
        pack(&v);
    }

    // Static method for backward compatibility with old code
    static void set(hkPackedUnitVector* p, float x, float y, float z, float w) {
        p->set(x, y, z, static_cast<unsigned int>(w));
    }

    // IDA: ?pack@?$hkPackedUnitVector@$02@@QEAAXAEBVhkVector4@@@Z @ 0x140276140
    void pack(const hkVector4* vIn) {
        // Multiply by constant 46 (scaling factor)
        const hkVector4* scaleConst = hkVector4::getConstant<46>();
        hkVector4 v;
        v.setMul(vIn, scaleConst);

        // Convert to int32
        hkIntVector iv32;
        iv32.setConvertF32toS32(&v);

        // Add offset constant
        static hkIntVector m_offset;
        iv32.setAddU32(&iv32, &m_offset);

        // Extract 16-bit values
        m_vec[0] = iv32.getU16<1>();
        m_vec[1] = iv32.getU16<3>();
        m_vec[2] = iv32.getU16<5>();
    }
};

// hkQTransform - Quaternion transform
// IDA: Uses hkQuaternion for rotation (stored as hkVector4 internally)
class hkQTransform {
public:
    hkVector4 m_translation;
    hkVector4 m_rotation;  // Quaternion (x, y, z, w) stored as hkVector4

    hkQTransform() {}

    // IDA: ??4hkQTransform@@QEAAAEAV0@AEBV0@@Z @ 0x1401ade00
    hkQTransform& operator=(const hkQTransform& that) {
        m_rotation = that.m_rotation;
        m_translation = that.m_translation;
        return *this;
    }

    // IDA: ?setIdentity@hkQTransform@@QEAAXXZ @ 0x1402f8540
    void setIdentity() {
        m_rotation.x = 0.0f;
        m_rotation.y = 0.0f;
        m_rotation.z = 0.0f;
        m_rotation.w = 1.0f;
        m_translation.x = 0.0f;
        m_translation.y = 0.0f;
        m_translation.z = 0.0f;
        m_translation.w = 0.0f;
    }

    // Static version for compatibility
    static void setIdentity(hkQTransform* t) {
        t->setIdentity();
    }

    // IDA: ?setTranslation@hkQTransform@@QEAAXAEBVhkVector4@@@Z @ 0x1402f8510
    static void setTranslation(hkQTransform* t, const hkVector4* v) {
        t->m_translation = *v;
    }

    // IDA: ?setTranslation@hkQTransform@@QEAAXAEBVhkVector4@@@Z - alias for hkSimdReal
    void setTranslation(const hkVector4* t) {
        hkVector4::setAll(&m_translation, reinterpret_cast<const hkSimdReal*>(t));
    }

    // IDA: ?isOk@hkQTransform@@QEBA_NXZ @ 0x1409dbe20
    // Returns true if both translation and rotation are valid (no NaN)
    bool isOk() const {
#ifdef _WIN32
        // Check translation for NaN
        if ((_mm_movemask_ps(_mm_cmpunord_ps(m_translation.m_quad, _mm_setzero_ps())) & 7) != 0) {
            return false;
        }
        // Check rotation quaternion for NaN
        if (_mm_movemask_ps(_mm_cmpunord_ps(m_rotation.m_quad, _mm_setzero_ps())) != 0) {
            return false;
        }
        // Check if rotation quaternion length is close to 1 (epsilon = 0.001)
        __m128 sq = _mm_mul_ps(m_rotation.m_quad, m_rotation.m_quad);
        __m128 sum1 = _mm_add_ps(_mm_shuffle_ps(sq, sq, 78), sq);  // x+y, z+w
        __m128 sum2 = _mm_shuffle_ps(sum1, sum1, 177);  // swap low and high
        float lenSq = _mm_cvtss_f32(sum1) + _mm_cvtss_f32(sum2);
        float diff = lenSq - 1.0f;
        return (diff * diff) < 0.001f;
#else
        // Check translation for finite values
        if (!std::isfinite(m_translation.x) || !std::isfinite(m_translation.y) || !std::isfinite(m_translation.z)) {
            return false;
        }
        // Check rotation for finite values
        if (!std::isfinite(m_rotation.x) || !std::isfinite(m_rotation.y) ||
            !std::isfinite(m_rotation.z) || !std::isfinite(m_rotation.w)) {
            return false;
        }
        // Check if rotation quaternion length is close to 1
        float lenSq = m_rotation.x * m_rotation.x + m_rotation.y * m_rotation.y +
                      m_rotation.z * m_rotation.z + m_rotation.w * m_rotation.w;
        float diff = lenSq - 1.0f;
        return (diff * diff) < 0.001f;
#endif
    }

    // IDA: ?isApproximatelyEqual@hkQTransform@@QEBA_NAEBV1@M@Z @ 0x1409dbe60
    bool isApproximatelyEqual(const hkQTransform& other, float epsilon) const;

    // IDA: ?setInverse@hkQTransform@@QEAAXAEBV1@@Z @ 0x1409dbee0
    void setInverse(const hkQTransform& qt);

    // IDA: ?setMul@hkQTransform@@QEAAXAEBV1@0@Z @ 0x1409dbf80
    void setMul(const hkQTransform& t1, const hkQTransform& t2);
};

// hkPackedVector3 - Packed 16-bit vector with shared exponent
// IDA: ?pack@hkPackedVector3@@QEAAXAEBVhkVector4@@@Z @ 0x14078ef80
class hkPackedVector3 {
public:
    unsigned short m_values[4];  // x, y, z components and shared exponent

    // IDA: ?pack@hkPackedVector3@@QEAAXAEBVhkVector4@@@Z @ 0x14078ef80
    void pack(const hkVector4& vIn);
};

// hkPackedVector8_3 - Packed 8-bit vector with shared exponent
// IDA: ?pack@hkPackedVector8_3@@QEAAXAEBVhkVector4@@@Z @ 0x14078f020
class hkPackedVector8_3 {
public:
    unsigned char m_values[4];  // x, y, z components and shared exponent

    // IDA: ?pack@hkPackedVector8_3@@QEAAXAEBVhkVector4@@@Z @ 0x14078f020
    void pack(const hkVector4& vIn);
};

// hkBitField - Bit field container
// IDA: ??1hkBitField@@QEAA@XZ @ 0x140791fe0
class hkBitField {
public:
    struct Words {
        unsigned int* m_data;
        int m_size;
        int m_capacityAndFlags;  // Negative = external storage
    } m_words;

    // IDA: ??1hkBitField@@QEAA@XZ @ 0x140791fe0
    ~hkBitField();

    // IDA: ?staticClass@hkBitField@@SAAEBVhkClass@@XZ @ 0x140791f60
    static const hkClass* staticClass();
};

// hkQuaternion - Quaternion (forward declaration, defined later)
struct hkQuaternion;

// hkRotation - 3x3 rotation matrix (forward declaration, defined later)
class hkRotation;

// hkMatrix3 - General 3x3 matrix
// IDA: ?transpose@hkMatrix3@@QEAAXXZ @ 0x1407ad7e0
class hkMatrix3 {
public:
    hkVector4 m_col0;  // Column 0
    hkVector4 m_col1;  // Column 1
    hkVector4 m_col2;  // Column 2

    hkMatrix3() {
        m_col0.x = 1.0f; m_col0.y = 0.0f; m_col0.z = 0.0f; m_col0.w = 0.0f;
        m_col1.x = 0.0f; m_col1.y = 1.0f; m_col1.z = 0.0f; m_col1.w = 0.0f;
        m_col2.x = 0.0f; m_col2.y = 0.0f; m_col2.z = 1.0f; m_col2.w = 0.0f;
    }

    // IDA: ?transpose@hkMatrix3@@QEAAXXZ @ 0x1407ad7e0
    void transpose();

    // IDA: ?isOk@hkMatrix3@@QEBA_NXZ @ 0x1407ad820
    bool isOk() const;

    // IDA: ?setMulInverseMul@hkMatrix3@@QEAAXAEBVhkRotation@@AEBV1@@Z @ 0x1407ad860
    void setMulInverseMul(const hkRotation& bTa, const hkMatrix3& bTc);

    // IDA: ?mul@hkMatrix3@@QEAAXAEBVhkSimdReal@@@Z @ 0x1407ada10
    void mul(const hkSimdReal& scale);

    // IDA: ?setCrossSkewSymmetric@hkMatrix3@@QEAAXAEBVhkVector4@@@Z @ 0x1407ada40
    void setCrossSkewSymmetric(const hkVector4& r);

    // IDA: ?add@hkMatrix3@@QEAAXAEBV1@@Z @ 0x1407adae0
    void add(const hkMatrix3& a);

    // IDA: ?sub@hkMatrix3@@QEAAXAEBV1@@Z @ 0x1407adb10
    void sub(const hkMatrix3& a);

    // IDA: ?setTranspose@hkMatrix3@@QEAAXAEBV1@@Z @ 0x1407ade00
    void setTranspose(const hkMatrix3& s);

    // IDA: ?setMul@hkMatrix3@@QEAAXAEBV1@0@Z @ 0x1407ade40
    void setMul(const hkMatrix3& aTb, const hkMatrix3& bTc);

    // IDA: ?mul@hkMatrix3@@QEAAXAEBV1@@Z @ 0x1407ae2f0
    void mul(const hkMatrix3& a);

    // IDA: ?frobeniusNormSqr@hkMatrix3@@QEBA?AVhkSimdReal@@XZ @ 0x1407ae410
    hkSimdReal frobeniusNormSqr() const;

    // IDA: ?getDeterminant@hkMatrix3@@QEBA?BVhkSimdReal@@XZ @ 0x1407ae330
    hkSimdReal getDeterminant() const;

    // IDA: ?isApproximatelyEqualSimd@hkMatrix3@@QEBA_NAEBV1@AEBVhkSimdReal@@@Z @ 0x1407ae010
    bool isApproximatelyEqualSimd(const hkMatrix3& m, const hkSimdReal& epsilon) const;

    // IDA: ?setTransposeMul@hkMatrix3@@QEAAXAEBV1@0@Z @ 0x1407ad8d0
    void setTransposeMul(const hkMatrix3& a, const hkMatrix3& b);

    // IDA: ?setMul@hkMatrix3@@QEAAXAEBVhkSimdReal@@AEBV1@@Z @ 0x1407adab0
    void setMul(const hkSimdReal& scale, const hkMatrix3& a);

    // IDA: ?addMul@hkMatrix3@@QEAAXAEBVhkSimdReal@@AEBV1@@Z @ 0x1407adf50
    void addMul(const hkSimdReal& scale, const hkMatrix3& a);

    // IDA: ?isApproximatelyEqual@hkMatrix3@@QEBA_NAEBV1@M@Z @ 0x1407adf90
    bool isApproximatelyEqual(const hkMatrix3& m, float epsilon) const;

    // IDA: ?isSymmetric@hkMatrix3@@QEBA_NM@Z @ 0x1407ae080
    bool isSymmetric(float epsilon) const;

    // IDA: ?invert@hkMatrix3@@QEAA?AUhkResult@@M@Z @ 0x1407ae0e0
    hkResult invert(float epsilon);

    // IDA: ?invertSymmetric@hkMatrix3@@QEAAXXZ @ 0x1407ae1f0
    void invertSymmetric();

    // IDA: ?setMulInverse@hkMatrix3@@QEAAXAEBV1@AEBVhkRotation@@@Z @ 0x1407adea0
    void setMulInverse(const hkMatrix3& aTb, const hkRotation& cTb);

    // IDA: ?changeBasis@hkMatrix3@@QEAAXAEBVhkRotation@@@Z @ 0x1407ae590
    void changeBasis(const hkRotation& r);

    // IDA: ?diagonalizeSymmetric@hkMatrix3@@QEBA?AUhkResult@@AEAVhkRotation@@AEAVhkVector4@@HM@Z @ 0x1407ae5d0
    // Jacobi eigenvalue decomposition for symmetric matrices
    hkResult diagonalizeSymmetric(hkRotation& eigenVec, hkVector4& eigenVal, int maxIter, float epsilon) const;

    // IDA: ?diagonalizeSymmetricApproximation@hkMatrix3@@QEBAXAEAVhkRotation@@AEAVhkVector4@@H@Z @ 0x1407ae9d0
    void diagonalizeSymmetricApproximation(hkRotation& eigenVec, hkVector4& eigenVal, int maxIter) const;

    // IDA: ?diagonalizeSymmetricWarmStart@hkMatrix3@@QEBA?AUhkResult@@AEAV1@AEAVhkVector4@@HM@Z @ 0x1407aed20
    hkResult diagonalizeSymmetricWarmStart(hkMatrix3& eigenVec, hkVector4& eigenVal, int maxIter, float epsilon) const;
};

// IDA: hkMatrix3_setMulMat3Mat3 @ 0x1407adda0
// Helper function for matrix multiplication
void hkMatrix3_setMulMat3Mat3(hkMatrix3* result, const hkMatrix3* aTb, const hkMatrix3* bTc);

// hkRotation - 3x3 rotation matrix
class hkRotation {
public:
    hkVector4 m_col0;  // Column 0
    hkVector4 m_col1;  // Column 1
    hkVector4 m_col2;  // Column 2

    hkRotation() {
        m_col0.x = 1.0f; m_col0.y = 0.0f; m_col0.z = 0.0f; m_col0.w = 0.0f;
        m_col1.x = 0.0f; m_col1.y = 1.0f; m_col1.z = 0.0f; m_col1.w = 0.0f;
        m_col2.x = 0.0f; m_col2.y = 0.0f; m_col2.z = 1.0f; m_col2.w = 0.0f;
    }

    // IDA: ?set@hkRotation@@QEAAXAEBVhkQuaternion@@@Z @ 0x140799ba0
    void set(const hkQuaternion& qi);

    // IDA: ?setAxisAngle@hkRotation@@QEAAXAEBVhkVector4@@M@Z @ 0x140799c40
    void setAxisAngle(const hkVector4& axis, float angle);

    // IDA: ?isOrthonormal@hkRotation@@QEBA_NM@Z @ 0x140799c70
    bool isOrthonormal(float epsilon = 0.00001f) const;

    // IDA: ?isOk@hkRotation@@QEBA_NXZ @ 0x140799d80
    bool isOk() const;

    // IDA: ?renormalize@hkRotation@@QEAAXXZ @ 0x140799dc0
    void renormalize();

    // Matrix multiplication (reuses hkMatrix3::setMul via reinterpret_cast)
    void setMul(const hkRotation& a, const hkRotation& b) {
        reinterpret_cast<hkMatrix3*>(this)->setMul(
            *reinterpret_cast<const hkMatrix3*>(&a),
            *reinterpret_cast<const hkMatrix3*>(&b));
    }

    // Approximate equality check (reuses hkMatrix3::isApproximatelyEqualSimd)
    bool isApproximatelyEqualSimd(const hkRotation& r, const hkSimdReal& epsilon) const {
        return reinterpret_cast<const hkMatrix3*>(this)->isApproximatelyEqualSimd(
            *reinterpret_cast<const hkMatrix3*>(&r), epsilon);
    }
};

// hkMatrix4 - 4x4 matrix
// IDA: ?transpose@hkMatrix4@@QEAAXXZ @ 0x1407b2d10
class hkMatrix4 {
public:
    hkVector4 m_col0;  // Column 0
    hkVector4 m_col1;  // Column 1
    hkVector4 m_col2;  // Column 2
    hkVector4 m_col3;  // Column 3

    hkMatrix4() {
        m_col0.x = 1.0f; m_col0.y = 0.0f; m_col0.z = 0.0f; m_col0.w = 0.0f;
        m_col1.x = 0.0f; m_col1.y = 1.0f; m_col1.z = 0.0f; m_col1.w = 0.0f;
        m_col2.x = 0.0f; m_col2.y = 0.0f; m_col2.z = 1.0f; m_col2.w = 0.0f;
        m_col3.x = 0.0f; m_col3.y = 0.0f; m_col3.z = 0.0f; m_col3.w = 1.0f;
    }

    // IDA: ?transpose@hkMatrix4@@QEAAXXZ @ 0x1407b2d10
    void transpose();

    // IDA: ?isOk@hkMatrix4@@QEBA_NXZ @ 0x1407b2d70
    bool isOk() const;

    // IDA: ?add@hkMatrix4@@QEAAXAEBV1@@Z @ 0x1407b2dc0
    void add(const hkMatrix4& a);

    // IDA: ?sub@hkMatrix4@@QEAAXAEBV1@@Z @ 0x1407b2e00
    void sub(const hkMatrix4& a);

    // IDA: ?setTransposeMul@hkMatrix4@@QEAAXAEBV1@0@Z @ 0x1407b2e40
    void setTransposeMul(const hkMatrix4& a, const hkMatrix4& b);

    // IDA: ?setInverse@hkMatrix4@@QEAA?AUhkResult@@AEBV1@M@Z @ 0x1407b3070
    hkResult setInverse(const hkMatrix4& a, float epsilon);

    // IDA: ?set@hkMatrix4@@QEAAXAEBVhkQTransform@@@Z @ 0x1407b3310
    void set(const hkQTransform& q);

    // IDA: ?invert@hkMatrix4@@QEAA?AUhkResult@@M@Z @ 0x1407b33b0
    hkResult invert(float epsilon);

    // IDA: ?isAffineTransformation@hkMatrix4@@QEBAIXZ @ 0x1407b3400
    unsigned int isAffineTransformation() const;

    // IDA: ?setMulInverseAffine@hkMatrix4@@QEAAXAEBV1@0@Z @ 0x1407b3450
    void setMulInverseAffine(const hkMatrix4& a, const hkMatrix4& b);

    // IDA: ?setMulInverse@hkMatrix4@@QEAAXAEBV1@0@Z @ 0x1407b35c0
    void setMulInverse(const hkMatrix4& a, const hkMatrix4& b);

    // IDA: ?get4x4RowMajor@hkMatrix4@@QEBAXPEIAM@Z @ 0x1407b3750
    void get4x4RowMajor(float* d) const;

    // IDA: ?get4x4RowMajor@hkMatrix4@@QEBAXPEIAN@Z @ 0x1407b37b0
    void get4x4RowMajor(double* d) const;

    // IDA: ?set4x4RowMajor@hkMatrix4@@QEAAXPEBM@Z @ 0x1407b3840
    void set4x4RowMajor(const float* p);

    // IDA: ?set4x4RowMajor@hkMatrix4@@QEAAXPEBN@Z @ 0x1407b38a0
    void set4x4RowMajor(const double* p);

    // IDA: ?get4x4ColumnMajor@hkMatrix4@@QEBAXPEIAM@Z @ 0x1407b3920
    void get4x4ColumnMajor(float* d) const;

    // IDA: ?set4x4ColumnMajor@hkMatrix4@@QEAAXPEBM@Z @ 0x1407b3950
    void set4x4ColumnMajor(const float* p);

    // IDA: ?get4x4ColumnMajor@hkMatrix4@@QEBAXPEIAN@Z @ 0x1407b3980
    void get4x4ColumnMajor(double* d) const;

    // IDA: ?set4x4ColumnMajor@hkMatrix4@@QEAAXPEBN@Z @ 0x1407b39f0
    void set4x4ColumnMajor(const double* p);
};

// hkQsTransform - forward declaration
class hkQsTransform;

// hkTransform - Havok transform (rotation matrix + translation)
// IDA: ?isOk@hkTransform@@QEBA_NXZ @ 0x14079a5c0
class hkTransform {
public:
    hkRotation m_rotation;     // 3x3 rotation matrix
    hkVector4 m_translation;   // Translation vector

    hkTransform() {
        m_translation.x = 0.0f;
        m_translation.y = 0.0f;
        m_translation.z = 0.0f;
        m_translation.w = 0.0f;
    }

    // IDA: ?isOk@hkTransform@@QEBA_NXZ @ 0x14079a5c0
    bool isOk() const {
#ifdef _WIN32
        // Check translation for NaN (lower 3 components)
        if ((_mm_movemask_ps(_mm_cmpunord_ps(m_translation.m_quad, _mm_setzero_ps())) & 7) != 0) {
            return false;
        }
#endif
        return m_rotation.isOk();
    }

    // IDA: ?setInverse@hkTransform@@QEAAXAEBV1@@Z @ 0x14079a600
    void setInverse(const hkTransform& t) {
        // Transpose rotation (inverse of orthogonal matrix)
        m_rotation.m_col0 = hkVector4(t.m_rotation.m_col0.x, t.m_rotation.m_col1.x, t.m_rotation.m_col2.x, 0.0f);
        m_rotation.m_col1 = hkVector4(t.m_rotation.m_col0.y, t.m_rotation.m_col1.y, t.m_rotation.m_col2.y, 0.0f);
        m_rotation.m_col2 = hkVector4(t.m_rotation.m_col0.z, t.m_rotation.m_col1.z, t.m_rotation.m_col2.z, 0.0f);

        // Negate translation and apply transposed rotation
        hkVector4 negTrans(-t.m_translation.x, -t.m_translation.y, -t.m_translation.z, 0.0f);
        m_translation.x = negTrans.x * m_rotation.m_col0.x + negTrans.y * m_rotation.m_col1.x + negTrans.z * m_rotation.m_col2.x;
        m_translation.y = negTrans.x * m_rotation.m_col0.y + negTrans.y * m_rotation.m_col1.y + negTrans.z * m_rotation.m_col2.y;
        m_translation.z = negTrans.x * m_rotation.m_col0.z + negTrans.y * m_rotation.m_col1.z + negTrans.z * m_rotation.m_col2.z;
        m_translation.w = 0.0f;
    }

    // Transform a point by this transform
    void transform(const hkVector4& in, hkVector4& out) const {
        // Apply rotation
        out.x = in.x * m_rotation.m_col0.x + in.y * m_rotation.m_col1.x + in.z * m_rotation.m_col2.x + m_translation.x;
        out.y = in.x * m_rotation.m_col0.y + in.y * m_rotation.m_col1.y + in.z * m_rotation.m_col2.y + m_translation.y;
        out.z = in.x * m_rotation.m_col0.z + in.y * m_rotation.m_col1.z + in.z * m_rotation.m_col2.z + m_translation.z;
        out.w = in.w;  // Preserve w component
    }

    // IDA: ?setMul@hkTransform@@QEAAXAEBV1@0@Z @ 0x14079a670
    void setMul(const hkTransform& aTb, const hkTransform& bTc);

    // IDA: ?setMulEq@hkTransform@@QEAAXAEBV1@@Z @ 0x14079a6e0
    void setMulEq(const hkTransform& bTc);

    // IDA: ?setMul@hkTransform@@QEAAXAEBVhkQsTransform@@AEBV1@@Z @ 0x14079a760
    void setMul(const hkQsTransform& aTb, const hkTransform& bTc);

    // IDA: ?setMulInverseMul@hkTransform@@QEAAXAEBV1@0@Z @ 0x14079a8b0
    void setMulInverseMul(const hkTransform& bTa, const hkTransform& bTc);

    // IDA: ?setMulMulInverse@hkTransform@@QEAAXAEBV1@0@Z @ 0x14079a990
    void setMulMulInverse(const hkTransform& wTa, const hkTransform& wTb);

    // IDA: ?isApproximatelyEqual@hkTransform@@QEBA_NAEBV1@M@Z @ 0x14079aa70
    bool isApproximatelyEqual(const hkTransform& t, float epsilon) const;

    // IDA: ?isApproximatelyEqualSimd@hkTransform@@QEBA_NAEBV1@AEBVhkSimdReal@@@Z @ 0x14079aaf0
    bool isApproximatelyEqualSimd(const hkTransform& t, const hkSimdReal& epsilon) const;

    // IDA: ?get4x4ColumnMajor@hkTransform@@QEBAXPEIAM@Z @ 0x14079ab60
    void get4x4ColumnMajor(float* d) const;

    // IDA: ?set4x4ColumnMajor@hkTransform@@QEAAXPEBM@Z @ 0x14079ac50
    void set4x4ColumnMajor(const float* p);

    // IDA: ?get4x4ColumnMajor@hkTransform@@QEBAXPEIAN@Z @ 0x14079abc0
    // Stores as doubles (for double-precision output)
    void get4x4ColumnMajor(double* d) const;

    // IDA: ?set4x4ColumnMajor@hkTransform@@QEAAXPEBN@Z @ 0x14079acb0
    // Loads from doubles (for double-precision input)
    void set4x4ColumnMajor(const double* p);
};

// hkPseudoRandomGenerator - Random number generator
// IDA: size = 8 bytes (m_seed + m_current)
class hkPseudoRandomGenerator {
public:
    unsigned int m_seed;
    unsigned int m_current;

    hkPseudoRandomGenerator() : m_seed(0), m_current(0) {}

    // IDA: ??0hkPseudoRandomGenerator@@QEAA@H@Z @ 0x1402f85a0
    hkPseudoRandomGenerator(unsigned int s) {
        m_seed = s;
        m_current = s;
    }

    // IDA: ?getRand32@hkPseudoRandomGenerator@@QEAAIXZ @ 0x1402f8a40
    unsigned int getRand32() {
        m_current = 1664525 * m_current + 1013904223;
        return m_current;
    }

    // IDA: ?getRandReal01@hkPseudoRandomGenerator@@QEAAMXZ @ 0x1402f8a70
    float getRandReal01() {
        return static_cast<float>(static_cast<int>(getRand32())) * 2.3283064e-10f;
    }

    // IDA: ?getRandReal11@hkPseudoRandomGenerator@@QEAAMXZ @ 0x1402f8ab0
    float getRandReal11() {
        return (2.0f * getRandReal01()) - 1.0f;
    }

    // IDA: ?getRandomVector11@hkPseudoRandomGenerator@@QEAAXAEAVhkVector4@@@Z @ 0x1402f8ae0
    void getRandomVector11(hkVector4* vecOut) {
        vecOut->x = getRandReal11();
        vecOut->y = getRandReal11();
        vecOut->z = getRandReal11();
        vecOut->w = getRandReal11();
    }
};

// ============================================================================
// Havok AI Types
// ============================================================================

// Forward declarations for AI types
class hkaiNavMeshInstance;

// hkaiPath::PathPoint - Path point
namespace hkaiPath {
    struct PathPoint {
        hkVector4 m_position;
        int m_faceKey;
    };
}

// hkaiAstarOutputParameters - A* pathfinding output parameters
// IDA: ??0hkaiAstarOutputParameters@@QEAA@XZ @ 0x140276e40
struct hkaiAstarOutputParameters {
    int m_numIterations;
    int m_goalIndex;
    hkEnum<int, int> m_status;         // IDA: uses hkEnum with CONSTRAINTS_NONE initial value
    hkEnum<int, int> m_terminationCause;  // IDA: uses hkEnum with CONSTRAINTS_NONE initial value

    hkaiAstarOutputParameters() {
        m_numIterations = 0;
        m_goalIndex = -1;
        m_status = 0;      // CONSTRAINTS_NONE = 0
        m_terminationCause = 0;
    }
};

// hkaiAvoidanceSolver - Avoidance solver
namespace hkaiAvoidanceSolver {

    // Kinematic constraint types
    enum KinematicConstraintType {
        CONSTRAINTS_NONE = 0,
        CONSTRAINTS_LINEAR_ONLY = 1,
        CONSTRAINTS_ANGULAR_ONLY = 2,
        CONSTRAINTS_LINEAR_AND_ANGULAR = 3
    };

    // Nearby boundaries search type
    enum NearbyBoundariesSearchType {
        SEARCH_NONE = 0
    };

    // MovementProperties - Movement properties for avoidance solver
    // IDA: ??0MovementProperties@hkaiAvoidanceSolver@@QEAA@XZ @ 0x1402f8630
    struct MovementProperties {
        float m_minVelocity;
        float m_maxVelocity;
        float m_maxAcceleration;
        float m_maxDeceleration;
        float m_leftTurnRadius;
        float m_rightTurnRadius;
        float m_maxAngularVelocity;
        float m_maxAngularAcceleration;
        float m_maxTurnVelocity;
        hkEnum<KinematicConstraintType, unsigned char> m_kinematicConstraintType;

        MovementProperties() {
            m_minVelocity = 0.02f;
            m_maxVelocity = 5.6399999f;
            m_maxAcceleration = 20.0f;
            m_maxDeceleration = 30.0f;
            m_leftTurnRadius = -1.0f;
            m_rightTurnRadius = -1.0f;
            m_maxAngularVelocity = 3.926991f;
            m_maxAngularAcceleration = 30.0f * m_maxAngularVelocity;
            m_maxTurnVelocity = 0.5f * m_maxVelocity;
            m_kinematicConstraintType = CONSTRAINTS_LINEAR_ONLY;
        }
    };

    // AvoidanceProperties - Avoidance properties for character
    // IDA: ??0AvoidanceProperties@hkaiAvoidanceSolver@@QEAA@XZ @ 0x1402f8710
    struct AvoidanceProperties {
        float m_data[36]; // 0x90 bytes

        AvoidanceProperties() {
            memset(m_data, 0, sizeof(m_data));
        }

        static const AvoidanceProperties* Create(AvoidanceProperties* p) {
            new (p) AvoidanceProperties();
            return p;
        }
    };
}

// hkaiCharacter::Cinfo - Character configuration
namespace hkaiCharacter {
    struct Cinfo {
        const hkaiAvoidanceSolver::AvoidanceProperties* m_avoidanceProperties;

        Cinfo() : m_avoidanceProperties(nullptr) {}

        static void Init(Cinfo* c) {
            new (c) Cinfo();
        }
    };
}

// hkaiNavMeshQueryMediator - NavMesh query mediator
class hkaiNavMeshQueryMediator {
public:
    void* m_pNavMesh;

    hkaiNavMeshQueryMediator() : m_pNavMesh(nullptr) {}

    // IDA: getClosestPoint - find closest point on navmesh
    unsigned int getClosestPoint(const hkVector4* point, float radius, hkVector4* outPoint) {
        // Stub - returns face key
        if (outPoint) {
            *outPoint = *point;
        }
        return 1; // Valid face key
    }

    // QueryInputBase - Base class for query inputs
    struct QueryInputBase {
        const hkaiNavMeshInstance* m_instance;
        const hkTransform* m_localToWorldTransform;

        QueryInputBase() : m_instance(nullptr), m_localToWorldTransform(nullptr) {}

        // IDA: ?setInstanceAndTransform@QueryInputBase@hkaiNavMeshQueryMediator@@QEAAXPEBVhkaiNavMeshInstance@@@Z @ 0x140280e70
        void setInstanceAndTransform(const hkaiNavMeshInstance* instance);
    };

    // RaycastInput - Raycast query input
    // IDA: ??0RaycastInput@hkaiNavMeshQueryMediator@@QEAA@XZ @ 0x1402f89a0
    struct RaycastInput : public QueryInputBase {
        hkVector4 m_from;
        hkVector4 m_to;

        RaycastInput() : QueryInputBase() {
            // IDA: Initialize from and to with constant 32 (zero vector)
            const hkSimdReal* zeroReal = reinterpret_cast<const hkSimdReal*>(hkVector4::getConstant<32>());
            hkVector4::setAll(&m_from, zeroReal);
            hkVector4::setAll(&m_to, zeroReal);
        }
    };
};

// Forward declarations for types defined later
class hkaiSingleCharacterBehavior;
class hkaiPathFollowingBehavior;
struct hkaiWorld_Cinfo;

// hkFinishLoadedObjectFlag - Flag for serialization constructor
// Used to indicate object is being loaded from serialized data
// Moved here (originally after line 6154) to resolve forward-reference errors
struct hkFinishLoadedObjectFlag {
    bool m_finishing;  // IDA: checked in constructors - if true, perform post-serialization init
};

// ============================================================================
// hkaiConvexSilhouetteSet - Convex silhouette set for Havok AI
// IDA: ctor ??0hkaiConvexSilhouetteSet@@QEAA@XZ (no-op, just returns this)
// IDA: ctor with flag ??0hkaiConvexSilhouetteSet@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1408191c0 (no-op)
// ============================================================================
class hkaiConvexSilhouetteSet {
public:
    // IDA: m_vertexPool is at offset after vtable and referenced via p_m_cachedSilhouettes[-1]
    void* m_vertexPool;

    hkaiConvexSilhouetteSet() : m_vertexPool(nullptr) {}

    // IDA: hkaiConvexSilhouetteSet(hkFinishLoadedObjectFlag) is empty no-op
    hkaiConvexSilhouetteSet(hkFinishLoadedObjectFlag flag) : m_vertexPool(nullptr) {
        (void)flag;
    }
};

// ============================================================================
// hkaiSilhouetteGenerator - Base class for silhouette generators
// IDA: ctor ??0hkaiSilhouetteGenerator@@QEAA@XZ - sets vtable, zeros fields
// IDA: ctor with flag ??0hkaiSilhouetteGenerator@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1408162b0
// In IDA: p_m_name[-1] = vtable; then calls hkaiConvexSilhouetteSet::hkaiConvexSilhouetteSet(&m_cachedSilhouettes, flag)
// p_m_cachedSilhouettes[-1] = m_vertexPool offset, so vtable is at m_vertexPool (offset 0), meaning vtable at offset 0
// ============================================================================
class hkaiSilhouetteGenerator {
public:
    // vtable pointer at offset 0 (before m_vertexPool)
    void* __vftable;
    void* m_vertexPool;
    hkaiConvexSilhouetteSet m_cachedSilhouettes;

    hkaiSilhouetteGenerator() : __vftable(nullptr), m_vertexPool(nullptr) {}

    hkaiSilhouetteGenerator(hkFinishLoadedObjectFlag flag) : __vftable(nullptr), m_vertexPool(nullptr) {
        // IDA: sets vtable then calls hkaiConvexSilhouetteSet ctor
        (void)flag;
    }

    static void setLazyRecomputeDisplacementThreshold(void* gen, float threshold) {
        // Stub
    }
};

// hkaiWorld - Havok AI World
// IDA: ctor ??0hkaiWorld@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1407f64d0
// IDA: vtable at offset +16 (p_m_defaultPathfindingInput[-2])
class hkaiWorld {
public:
    // Two pointer-sized slots before m_defaultPathfindingInput where vtable goes at offset +16
    void* _pad0;
    void* _pad1;
    // IDA: hkaiPathfindingUtil::FindPathInput::FindPathInput(base + 0xE0, flag)
    // Stored as opaque bytes because hkaiPathfindingUtil is defined later in this header
    // FindPathInput starts at offset 0xE0, size 0xC0 (verified from IDA ctor at 0x1407f64d0)
    char m_defaultPathfindingInput[0xC0];
    // IDA: hkaiVolumePathfindingUtil::FindPathInput::FindPathInput(base + 0x1A0, flag)
    // Stored as opaque bytes because hkaiVolumePathfindingUtil is defined later in this header
    // FindPathInput starts at offset 0x1A0, size 0xC0 (verified from IDA ctor at 0x1407f64d0)
    char m_defaultVolumePathfindingInput[0xC0];

    void* m_pNavMeshCollection;
    void* m_pSilhouetteGenerators;

    // IDA: zeros in constructor: m_agentInfo.m_diameter = -0.0, m_goalFaceKeys.m_size = 0,
    //   m_maxNumberOfIterations = 0, m_searchParameters.m_heuristicWeight = 0, m_maximumPathLength = 0.0,
    //   m_searchSphereRadius = -0.0, m_searchCapsuleRadius = 0,
    //   m_maxSearchStateSizeBytes = 0, m_maxHierarchyOpenSetSizeBytes = 0x80000000
    float m_agentInfoDiameter;
    int m_goalFaceKeysSize;
    int m_maxNumberOfIterations;
    float m_heuristicWeight;
    float m_maximumPathLength;
    float m_searchSphereRadius;
    float m_searchCapsuleRadius;
    int m_maxSearchStateSizeBytes;
    unsigned int m_maxHierarchyOpenSetSizeBytes;

    hkaiWorld() : _pad0(nullptr), _pad1(nullptr),
                  m_pNavMeshCollection(nullptr), m_pSilhouetteGenerators(nullptr),
                  m_agentInfoDiameter(0), m_goalFaceKeysSize(0), m_maxNumberOfIterations(0),
                  m_heuristicWeight(0), m_maximumPathLength(0), m_searchSphereRadius(0),
                  m_searchCapsuleRadius(0), m_maxSearchStateSizeBytes(0),
                  m_maxHierarchyOpenSetSizeBytes(0x80000000) {}

    // IDA: getDynamicQueryMediator
    hkaiNavMeshQueryMediator* getDynamicQueryMediator() {
        return reinterpret_cast<hkaiNavMeshQueryMediator*>(&m_pNavMeshCollection);
    }

    // IDA: getStreamingCollection
    void* getStreamingCollection() {
        return m_pNavMeshCollection;
    }

    // IDA: addSilhouetteGenerator
    void addSilhouetteGenerator(void* generator) {
        // Stub - add silhouette generator
    }

    // IDA: stepSilhouettes - step silhouettes for pathfinding
    void stepSilhouettes(void* pGenerator) {
        // Stub - updates navigation mesh silhouettes for dynamic obstacles
    }
};

// hkaiNavMeshInstance - NavMesh instance
// IDA: ctor ??0hkaiNavMeshInstance@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x14080b230
// IDA: size = 0x68 (104 bytes) including source pointer at offset 0x60
// Layout: vtable at +0, 10 field pairs at offsets 0x08-0x58, source pointer at 0x60
class hkaiNavMeshInstance {
public:
    // vtable at offset 0
    void* __vftable;
    // Field pairs copied from serialized source (10 QWORD/DWORD pairs)
    // IDA decompile shows specific source offsets mapped to destination:
    // this+0x08 <- src+0x10 (8 bytes)
    std::uint64_t m_field00;
    // this+0x10 <- src+0x18 (4 bytes)
    std::uint32_t m_field01;
    // this+0x14 padding
    std::uint32_t _pad14;
    // this+0x18 <- src+0x20 (8 bytes, but IDA shows *((QWORD*)this + 4) which is 0x20)
    std::uint64_t m_field02;
    // this+0x20 <- src+0x28 (4 bytes)
    std::uint32_t m_field03;
    // this+0x24 padding
    std::uint32_t _pad24;
    // this+0x28 <- src+0x30 (8 bytes)
    std::uint64_t m_field04;
    // this+0x30 <- src+0x38 (4 bytes)
    std::uint32_t m_field05;
    // this+0x34 padding
    std::uint32_t _pad34;
    // this+0x38 <- src+0x50 (8 bytes)
    std::uint64_t m_field06;
    // this+0x40 <- src+0x70 (4 bytes)
    std::uint32_t m_field07;
    // this+0x44 padding
    std::uint32_t _pad44;
    // this+0x48 <- src+0x60 (8 bytes)
    std::uint64_t m_field08;
    // this+0x50 <- src+0x74 (4 bytes)
    std::uint32_t m_field09;
    // this+0x54 padding
    std::uint32_t _pad54;
    // this+0x58 <- src+0x? (remaining fields)
    std::uint64_t m_field10;
    // this+0x60: source pointer (used during serialization)
    void* m_sourcePointer;

    hkaiNavMeshInstance() : __vftable(nullptr), m_field00(0), m_field01(0), _pad14(0),
                            m_field02(0), m_field03(0), _pad24(0), m_field04(0),
                            m_field05(0), _pad34(0), m_field06(0), m_field07(0),
                            _pad44(0), m_field08(0), m_field09(0), _pad54(0),
                            m_field10(0), m_sourcePointer(nullptr) {}

    // IDA: ?getTransform@hkaiNavMeshInstance@@QEBAAEBVhkTransform@@XZ @ 0x140280e50
    const hkTransform& getTransform() const {
        // Return transform stored in fields (m_field02/03 area based on offset analysis)
        return *reinterpret_cast<const hkTransform*>(&m_field02);
    }

    static void Create(void* p) {
        new (p) hkaiNavMeshInstance();
    }

    static void init(hkaiNavMeshInstance* inst) {
        // Stub
    }
};

// hkaiPointCloudSilhouetteGenerator - Point cloud silhouette generator
class hkaiPointCloudSilhouetteGenerator {
public:
    // IDA shows first member is m_isEnabled
    struct {
        bool m_bool;
    } m_isEnabled;

    char m_data[206]; // Remaining data to total 208 bytes

    hkaiPointCloudSilhouetteGenerator() {
        m_isEnabled.m_bool = false;
        memset(m_data, 0, sizeof(m_data));
    }

    static hkaiPointCloudSilhouetteGenerator* Create(void* p) {
        new (p) hkaiPointCloudSilhouetteGenerator();
        return reinterpret_cast<hkaiPointCloudSilhouetteGenerator*>(p);
    }

    // IDA: ?willGenerateSilhouettes@hkaiPointCloudSilhouetteGenerator@@UEBA_NAEBVhkaiNavMeshInstance@@AEBVhkVector4@@@Z @ 0x1407f4840
    bool willGenerateSilhouettes(const hkaiNavMeshInstance* instance, const hkVector4* pos) const {
        return m_isEnabled.m_bool != 0;
    }

    static void setLazyRecomputeDisplacementThreshold(hkaiPointCloudSilhouetteGenerator* gen, float threshold) {
        // Stub
    }

    static void setWeldTolerance(hkaiPointCloudSilhouetteGenerator* gen, float tolerance) {
        // Stub
    }

    static void setLocalPoints(hkaiPointCloudSilhouetteGenerator* gen, void* points) {
        // Stub
    }

    static void setTransform(hkaiPointCloudSilhouetteGenerator* gen, const hkQTransform* t) {
        // Stub
    }

    static void setEnabled(hkaiPointCloudSilhouetteGenerator* gen, bool enable) {
        gen->m_isEnabled.m_bool = enable;
    }

    static void setFromAabb(hkaiPointCloudSilhouetteGenerator* gen, void* aabb) {
        // Stub - set from axis-aligned bounding box
    }

    // IDA: ??3hkaiPointCloudSilhouetteGenerator@@SAXPEAX_K@Z @ 0x1402f8920
    static void operator delete(void* p, size_t nbytes) {
        if (!p) return;

        // Check if the allocation size is stored at offset +8 (2 bytes)
        // IDA pattern: check *(uint16_t*)(p + 8) == 0xFFFF
        uint16_t storedSize = *reinterpret_cast<uint16_t*>(static_cast<char*>(p) + 8);
        int actualSize;
        if (storedSize == 0xFFFF) {
            actualSize = static_cast<int>(nbytes);
        } else {
            actualSize = storedSize;
        }

        hkMemoryRouter* router = hkMemoryRouter::getInstance();
        hkMemoryAllocator* allocator = router->m_heap;
        allocator->blockFree(p, actualSize);
    }
};

// hkAabb - Axis-aligned bounding box
struct hkAabb {
    hkVector4 m_min;
    hkVector4 m_max;
};

// hkAabbUtil - AABB utility functions
// IDA: ?calcAabb@hkAabbUtil@@SAXPEBVhkVector4@@HAEAVhkAabb@@@Z @ 0x14078f630
namespace hkAabbUtil {
    // Calculate AABB from vertex array
    void calcAabb(const hkVector4* vertexArray, int numVertices, hkAabb& aabbOut);

    // Calculate AABB from pointer array of vertices
    void calcAabb(hkVector4** vertexArray, int numVertices, hkAabb& aabbOut);

    // Transform AABB by hkTransform
    void calcAabb(const hkTransform& BvToWorld, const hkAabb& aabb, hkAabb& aabbOut);

    // Transform AABB by hkTransform with extra radius
    void calcAabb(const hkTransform& BvToWorld, const hkAabb& aabb, const hkSimdReal& extraRadius, hkAabb& aabbOut);

    // Transform AABB by hkQsTransform
    void calcAabb(const hkQsTransform& bvToWorld, const hkAabb& aabb, hkAabb& aabbOut);

    // IDA: ?calcAabb@hkAabbUtil@@SAXAEBVhkQTransform@@AEBVhkAabb@@AEAV3@@Z @ 0x14078f9e0
    // Transform AABB by hkQTransform (quaternion transform, no scale)
    void calcAabb(const hkQTransform& bvToWorld, const hkAabb& aabb, hkAabb& aabbOut);

    // IDA: ?calcAabb@hkAabbUtil@@SAXPEBMHHAEAVhkAabb@@@Z @ 0x14078fae0
    // Calculate AABB from float array with striding
    void calcAabb(const float* vertexArray, int numVertices, int striding, hkAabb& aabbOut);
}

// Forward declarations
class hkLocalFrame;
class hkLocalFrameGroup;
class hkLocalFrameCollector;
class hkReferencedObject;

// hkArrayBase forward declaration
template<typename T>
class hkArrayBase;

// hkLocalFrame - Base class for local coordinate frames
// IDA: ?staticClass@hkLocalFrame@@SAAEBVhkClass@@XZ @ 0x140791570
class hkLocalFrame {
public:
    virtual ~hkLocalFrame() {}

    // Virtual interface
    virtual const hkLocalFrame* getParentFrame() const = 0;
    virtual void setParentFrame(const hkLocalFrame* parent) = 0;
    virtual const hkLocalFrameGroup* getGroup() const = 0;
    virtual void setGroup(const hkLocalFrameGroup* group) = 0;
    virtual void getLocalTransform(hkTransform* transform) const = 0;
    virtual void setLocalTransform(const hkTransform* transform) = 0;
    virtual void getLocalPosition(hkVector4* position) const = 0;
    virtual int getNumChildFrames() const = 0;
    virtual hkLocalFrame* getChildFrame(int i) = 0;
    virtual void getNearbyFrames(const hkVector4& target, float maxDistance, hkLocalFrameCollector* collector) const = 0;

    // IDA: ?getTransformToRoot@hkLocalFrame@@QEBAXAEAVhkTransform@@@Z @ 0x14079d420
    void getTransformToRoot(hkTransform* transform) const;

    // IDA: ?getPositionInRoot@hkLocalFrame@@QEBAXAEAVhkVector4@@@Z @ 0x14079d4b0
    void getPositionInRoot(hkVector4* position) const;

    // IDA: ?getDescendants@hkLocalFrame@@UEBAXAEAV?$hkArrayBase@PEBVhkLocalFrame@@@@AEAVhkMemoryAllocator@@@Z @ 0x14079d610
    virtual void getDescendants(hkArrayBase<const hkLocalFrame*>& descendants, hkMemoryAllocator* alloc) const;
};

// hkLocalFrameCollector - Collector for nearby frames
class hkLocalFrameCollector {
public:
    virtual ~hkLocalFrameCollector() {}
    virtual void addFrame(const hkLocalFrame* frame) = 0;
};

// hkSimpleLocalFrame - Simple implementation of hkLocalFrame
// IDA: ?staticClass@hkSimpleLocalFrame@@SAAEBVhkClass@@XZ
class hkSimpleLocalFrame : public hkLocalFrame {
public:
    hkTransform m_transform;
    const hkLocalFrame* m_parentFrame;
    hkLocalFrameGroup* m_group;
    hkLocalFrame** m_childrenData;  // Pointer to children array
    int m_childrenSize;
    int m_childrenCapacity;

    hkSimpleLocalFrame() : m_parentFrame(nullptr), m_group(nullptr), m_childrenData(nullptr), m_childrenSize(0), m_childrenCapacity(0) {}

    // IDA: ?getParentFrame@hkSimpleLocalFrame@@UEBAPEBVhkLocalFrame@@XZ @ 0x1407916f0
    const hkLocalFrame* getParentFrame() const override { return m_parentFrame; }

    // IDA: ?setParentFrame@hkSimpleLocalFrame@@UEAAXPEBVhkLocalFrame@@@Z @ 0x140791700
    void setParentFrame(const hkLocalFrame* parent) override { m_parentFrame = parent; }

    // IDA: ?getGroup@hkSimpleLocalFrame@@UEBAPEBVhkLocalFrameGroup@@XZ @ 0x140791710
    const hkLocalFrameGroup* getGroup() const override { return m_group; }

    // IDA: ?setGroup@hkSimpleLocalFrame@@UEAAXPEBVhkLocalFrameGroup@@@Z @ 0x14079d530
    void setGroup(const hkLocalFrameGroup* group) override;

    // IDA: ?getLocalTransform@hkSimpleLocalFrame@@UEBAXAEAVhkTransform@@@Z @ 0x14079d700
    void getLocalTransform(hkTransform* transform) const override { *transform = m_transform; }

    // IDA: ?setLocalTransform@hkSimpleLocalFrame@@UEAAXAEBVhkTransform@@@Z @ 0x14079d6d0
    void setLocalTransform(const hkTransform* transform) override { m_transform = *transform; }

    // IDA: ?getLocalPosition@hkSimpleLocalFrame@@UEBAXAEAVhkVector4@@@Z @ 0x14079d5d0
    void getLocalPosition(hkVector4* position) const override { *position = m_transform.m_translation; }

    // IDA: ?getNumChildFrames@hkSimpleLocalFrame@@UEBAHXZ @ 0x14079d5e0
    int getNumChildFrames() const override { return m_childrenSize; }

    // IDA: ?getChildFrame@hkSimpleLocalFrame@@UEBAPEAVhkLocalFrame@@H@Z @ 0x14079d5f0
    hkLocalFrame* getChildFrame(int i) override { return m_childrenData[i]; }

    // IDA: ?getNearbyFrames@hkSimpleLocalFrame@@UEBAXAEBVhkVector4@@MAEAVhkLocalFrameCollector@@@Z @ 0x14079d730
    void getNearbyFrames(const hkVector4& target, float maxDistance, hkLocalFrameCollector* collector) const override;
};

// hkQuaternion - Quaternion
struct hkQuaternion {
    hkVector4 m_vec;  // Internal vector representation (x, y, z, w)

    hkQuaternion() {
        m_vec.x = 0.0f;
        m_vec.y = 0.0f;
        m_vec.z = 0.0f;
        m_vec.w = 1.0f;
    }

    // IDA: ??4hkQuaternion@@QEAAXAEBV0@@Z @ 0x1401adeb0
    void operator=(const hkQuaternion& q) {
        m_vec = q.m_vec;
    }

    // IDA: ?setIdentity@hkQuaternion@@QEAAXXZ @ 0x1402f8e40
    void setIdentity() {
        m_vec.x = 0.0f;
        m_vec.y = 0.0f;
        m_vec.z = 0.0f;
        m_vec.w = 1.0f;
    }

    // Accessors for compatibility
    float& x() { return m_vec.x; }
    float& y() { return m_vec.y; }
    float& z() { return m_vec.z; }
    float& w() { return m_vec.w; }
    const float& x() const { return m_vec.x; }
    const float& y() const { return m_vec.y; }
    const float& z() const { return m_vec.z; }
    const float& w() const { return m_vec.w; }

    // IDA: ?isOk@hkQuaternion@@QEBAIM@Z @ 0x14078c8b0
    bool isOk(float epsilon = 0.01f) const {
        // Check for NaN
#ifdef _WIN32
        if (_mm_movemask_ps(_mm_cmpunord_ps(m_vec.m_quad, _mm_setzero_ps())) != 0) {
            return false;
        }
#endif
        // Check if length is close to 1
        float lenSq = m_vec.x * m_vec.x + m_vec.y * m_vec.y + m_vec.z * m_vec.z + m_vec.w * m_vec.w;
        float diff = lenSq - 1.0f;
        return (diff * diff) < epsilon;
    }

    // Get quaternion length
    float getLength() const {
        return std::sqrt(m_vec.x * m_vec.x + m_vec.y * m_vec.y + m_vec.z * m_vec.z + m_vec.w * m_vec.w);
    }

    // IDA: ?setFlippedRotation@hkQuaternion@@QEAAXAEBVhkVector4@@@Z @ 0x14078cac0
    void setFlippedRotation(const hkVector4& from) {
        // Complex SSE implementation - simplified stub
        // Computes quaternion that rotates from one direction to another
        m_vec.w = 1.0f;
        m_vec.x = 0.0f;
        m_vec.y = 0.0f;
        m_vec.z = 0.0f;
    }

    // IDA: ?setAxisAngle@hkQuaternion@@QEAAXAEBVhkVector4@@M@Z @ 0x14078c3c0
    void setAxisAngle(const hkVector4& axis, float angle);

    // IDA: ?setAxisAngle_Approximate@hkQuaternion@@QEAAXAEBVhkVector4@@AEBVhkSimdReal@@@Z @ 0x14078c3f0
    void setAxisAngle_Approximate(const hkVector4& axis, const hkSimdReal& angle);

    // Helper: simd real version (used internally)
    void setAxisAngle(const hkVector4& axis, const hkSimdReal& angle);

    // IDA: ?setFromEulerAngles@hkQuaternion@@QEAAXMMM@Z @ 0x14078c710
    void setFromEulerAngles(float roll, float pitch, float yaw);

    // IDA: ?setFromEulerAngles@hkQuaternion@@QEAAXAEBVhkSimdReal@@00@Z @ 0x14078c480
    void setFromEulerAngles(const hkSimdReal& roll, const hkSimdReal& pitch, const hkSimdReal& yaw);

    // IDA: ?setFromEulerAngles_Approximate@hkQuaternion@@QEAAXAEBVhkSimdReal@@00@Z @ 0x14078c770
    void setFromEulerAngles_Approximate(const hkSimdReal& roll, const hkSimdReal& pitch, const hkSimdReal& yaw);

    // IDA: ?setSlerp@hkQuaternion@@QEAAXAEBV1@0AEBVhkSimdReal@@@Z @ 0x14078d1b0
    void setSlerp(const hkQuaternion& q0, const hkQuaternion& q1, const hkSimdReal& t);

    // IDA: ?set@hkQuaternion@@QEAAXAEBVhkRotation@@@Z @ 0x14078c910
    void set(const hkRotation& r);

    // IDA: ?removeAxisComponent@hkQuaternion@@QEAAXAEBVhkVector4@@@Z @ 0x14078cbc0
    void removeAxisComponent(const hkVector4& axis);

    // IDA: ?decomposeRestAxis@hkQuaternion@@QEBAXAEBVhkVector4@@AEAV1@AEAVhkSimdReal@@@Z @ 0x14078cf60
    void decomposeRestAxis(const hkVector4& axis, hkQuaternion& restOut, hkSimdReal& angleOut) const;

    // IDA: ?setBarycentric@hkQuaternion@@QEAAXPEIBV1@AEBVhkVector4@@@Z @ 0x14078d530
    void setBarycentric(const hkQuaternion* qVerts, const hkVector4& vLambda);
};

// hkQsTransform - Transform with quaternion rotation and scale
// IDA: ?setFromTransformNoScale@hkQsTransform@@QEAAXAEBVhkTransform@@@Z @ 0x14079a080
class hkQsTransform {
public:
    hkQuaternion m_rotation;    // Rotation as quaternion
    hkVector4 m_translation;    // Translation
    hkVector4 m_scale;          // Scale (x, y, z, unused)

    hkQsTransform() {
        m_translation.x = 0.0f; m_translation.y = 0.0f; m_translation.z = 0.0f; m_translation.w = 0.0f;
        m_scale.x = 1.0f; m_scale.y = 1.0f; m_scale.z = 1.0f; m_scale.w = 1.0f;
    }

    // IDA: ?isOk@hkQsTransform@@QEBA_NM@Z @ 0x14079a130
    bool isOk(float epsilon = 0.00001f) const;

    // IDA: ?setFromTransformNoScale@hkQsTransform@@QEAAXAEBVhkTransform@@@Z @ 0x14079a080
    void setFromTransformNoScale(const hkTransform& transform) {
        // Set rotation from rotation matrix (stub - needs conversion)
        m_rotation.setIdentity();
        m_translation = transform.m_translation;
        // Set scale to (1, 1, 1) - g_vectorConstants[2] is likely identity scale
        m_scale.x = 1.0f; m_scale.y = 1.0f; m_scale.z = 1.0f; m_scale.w = 1.0f;
    }

    // IDA: ?setFromTransform@hkQsTransform@@QEAAXAEBVhkTransform@@@Z @ 0x14079a100
    void setFromTransform(const hkTransform& transform);

    // IDA: ?setFromTransform@hkQsTransform@@QEAAXAEBVhkQTransform@@@Z @ 0x14079a110
    void setFromTransform(const hkQTransform& qt) {
        m_rotation.m_vec = qt.m_rotation;
        m_translation = qt.m_translation;
        // Set scale to (1, 1, 1) - g_vectorConstants[2] is identity scale
        m_scale.x = 1.0f; m_scale.y = 1.0f; m_scale.z = 1.0f; m_scale.w = 1.0f;
    }

    // IDA: ?copyToTransformNoScale@hkQsTransform@@QEBAXAEAVhkTransform@@@Z @ 0x14079a0c0
    void copyToTransformNoScale(hkTransform& transformOut) const {
        // Set rotation matrix from quaternion (stub - needs conversion)
        // hkRotation::set(&transformOut.m_rotation, &m_rotation);
        transformOut.m_translation = m_translation;
    }

    // IDA: ?copyToTransform@hkQsTransform@@QEBAXAEAVhkTransform@@@Z @ 0x14079a270
    void copyToTransform(hkTransform& transformOut) const;

    // Transform a point by this transform (with scale)
    void transform(const hkVector4& in, hkVector4& out) const {
        // Apply scale first
        hkVector4 scaled(in.x * m_scale.x, in.y * m_scale.y, in.z * m_scale.z, in.w);

        // Apply rotation using quaternion
        // q * v * q^-1 formula for quaternion rotation
        hkVector4 qvec(m_rotation.m_vec.x, m_rotation.m_vec.y, m_rotation.m_vec.z, m_rotation.m_vec.w);

        // Cross product: qvec x scaled
        float cx = qvec.y * scaled.z - qvec.z * scaled.y;
        float cy = qvec.z * scaled.x - qvec.x * scaled.z;
        float cz = qvec.x * scaled.y - qvec.y * scaled.x;

        // 2.0 * cross
        float cx2 = cx * 2.0f;
        float cy2 = cy * 2.0f;
        float cz2 = cz * 2.0f;

        // dot = 2.0 * qvec.dot(scaled)
        float dot = 2.0f * (qvec.x * scaled.x + qvec.y * scaled.y + qvec.z * scaled.z);

        // rotated = scaled + cx2 * qvec + dot * m_rotation.m_vec.w - cx2 * scaled (simplified)
        // Using standard quaternion rotation formula
        float ww = m_rotation.m_vec.w * m_rotation.m_vec.w;
        float xx = qvec.x * qvec.x;
        float yy = qvec.y * qvec.y;
        float zz = qvec.z * qvec.z;

        out.x = scaled.x * (ww + xx - yy - zz) + 2.0f * (qvec.x * qvec.y * scaled.y - qvec.z * qvec.x * scaled.z) + 2.0f * m_rotation.m_vec.w * (qvec.y * scaled.z - qvec.z * scaled.y);
        out.y = scaled.y * (ww - xx + yy - zz) + 2.0f * (qvec.x * qvec.y * scaled.x + qvec.z * qvec.y * scaled.z) + 2.0f * m_rotation.m_vec.w * (qvec.z * scaled.x - qvec.x * scaled.z);
        out.z = scaled.z * (ww - xx - yy + zz) + 2.0f * (qvec.z * qvec.x * scaled.x - qvec.x * qvec.y * scaled.y) + 2.0f * m_rotation.m_vec.w * (qvec.x * scaled.y - qvec.y * scaled.x);
        out.w = in.w;

        // Apply translation
        out.x += m_translation.x;
        out.y += m_translation.y;
        out.z += m_translation.z;
    }

    // === Static batch operations ===

    // IDA: ?fastRenormalizeQuaternionBatch@hkQsTransform@@SAXPEAV1@I@Z @ 0x14079a320
    // Renormalizes quaternions in a batch of transforms using SIMD
    static void fastRenormalizeQuaternionBatch(hkQsTransform* poseOut, unsigned int numTransforms);

    // IDA: ?fastRenormalizeBatch@hkQsTransform@@SAXPEAV1@PEAMI@Z @ 0x14079a4c0
    // Renormalizes translations and scales by weight, then quaternions
    static void fastRenormalizeBatch(hkQsTransform* poseOut, float* weight, unsigned int numTransforms);

    // IDA: ?fastRenormalizeBatch@hkQsTransform@@SAXPEAV1@MI@Z @ 0x14079a530
    // Renormalizes translations and scales by single weight, then quaternions
    static void fastRenormalizeBatch(hkQsTransform* poseOut, float weight, unsigned int numTransforms);
};

// hkaiSilhouetteGenerator — setLazyRecomputeDisplacementThreshold was previously in a separate
// namespace that conflicted with the class above. It is now a static member of the class.
// See class hkaiSilhouetteGenerator for setLazyRecomputeDisplacementThreshold.

// ============================================================================
// Havok Memory Management
// ============================================================================

// hkBaseObject - Base object for Havok types
class hkBaseObject {
public:
    virtual ~hkBaseObject() {}
};

// hkReferencedObject - Reference counted object base
// IDA: size = 0x10 (16 bytes)
class hkReferencedObject : public hkBaseObject {
public:
    short m_memSizeAndFlags;   // IDA: -1 by default
    short m_referenceCount;    // IDA: 1 by default

    // LockMode enum for reference counting
    enum LockMode {
        LOCK_MODE_NONE = 0,
        LOCK_MODE_MANUAL = 1,
        LOCK_MODE_AUTO = 2
    };

    // IDA: ??0hkReferencedObject@@QEAA@XZ @ 0x140277850
    hkReferencedObject() {
        m_memSizeAndFlags = -1;
        m_referenceCount = 1;
    }

    // IDA: ??1hkReferencedObject@@UEAA@XZ @ 0x140277760
    virtual ~hkReferencedObject() {}

    // IDA: ??2hkReferencedObject@@SAPEAX_K@Z @ 0x1402f88e0
    static void* operator new(size_t nbytes) {
        // Stub - in real Havok, uses hkMemoryRouter
        return ::operator new(nbytes);
    }

    // Placement new for in-place construction
    static void* operator new(size_t nbytes, void* ptr) {
        return ptr;
    }

    static void addReference(hkReferencedObject* obj) {
        if (obj) obj->m_referenceCount++;
    }

    static void removeReference(hkReferencedObject* obj) {
        if (obj && --obj->m_referenceCount == 0) {
            delete obj;
        }
    }

    int getReferenceCount() const { return m_referenceCount; }

    // IDA: ?getClassType@hkReferencedObject@@UEBAPEBVhkClass@@XZ @ 0x14078b570
    virtual const hkClass* getClassType() const {
        return nullptr;
    }

    // IDA: ?addReferenceLockUnchecked@hkReferencedObject@@QEBAXXZ @ 0x14078b580
    void addReferenceLockUnchecked() const {
        if (m_memSizeAndFlags) {
            const_cast<hkReferencedObject*>(this)->m_referenceCount++;
        }
    }

    // IDA: ?removeReferenceLockUnchecked@hkReferencedObject@@QEBAXXZ @ 0x14078b5a0
    void removeReferenceLockUnchecked() const {
        if (m_memSizeAndFlags) {
            hkReferencedObject* self = const_cast<hkReferencedObject*>(this);
            if (--self->m_referenceCount == 0) {
                delete self;
            }
        }
    }

    // IDA: ?initializeLock@hkReferencedObject@@SAXXZ @ 0x14078b5e0
    static void initializeLock();

    // IDA: ?deinitializeLock@hkReferencedObject@@SAXXZ @ 0x14078b640
    static void deinitializeLock();

    // IDA: ?setLockMode@hkReferencedObject@@SAXW4LockMode@1@@Z @ 0x14078b670
    static void setLockMode(LockMode mode);

    // IDA: ?lockInit@hkReferencedObject@@SAXW4LockMode@1@@Z @ 0x14078b690
    static void lockInit(LockMode lockMode);

    // IDA: ?getLockMode@hkReferencedObject@@SA?AW4LockMode@1@XZ @ 0x14078b710
    static LockMode getLockMode();

    // IDA: ?lockAll@hkReferencedObject@@SAXXZ @ 0x14078b730
    static void lockAll();

    // IDA: ?unlockAll@hkReferencedObject@@SAXXZ @ 0x14078b790
    static void unlockAll();

    // IDA: ?addReference@hkReferencedObject@@QEBAXXZ @ 0x14078b7d0
    void addReference() const;

    // IDA: ?removeReference@hkReferencedObject@@QEBAXXZ @ 0x14078b8a0
    void removeReference() const;

    // IDA: ?addReferences@hkReferencedObject@@SAXPEBQEBV1@HH@Z @ 0x14078b9a0
    static void addReferences(hkReferencedObject** objects, int numObjects, int pointerStriding);

    // IDA: ?removeReferences@hkReferencedObject@@SAXPEBQEBV1@HH@Z @ 0x14078baa0
    static void removeReferences(hkReferencedObject** objects, int numObjects, int pointerStriding);

    // IDA: ?getLockCriticalSection@hkReferencedObject@@SAPEAVhkCriticalSection@@XZ @ 0x14078bb90
    static hkCriticalSection* getLockCriticalSection();
};

// hkLocalFrameGroup - Group of local frames
// IDA: ?staticClass@hkLocalFrameGroup@@SAAEBVhkClass@@XZ @ 0x140791580
// Inherits from hkReferencedObject for reference counting
class hkLocalFrameGroup : public hkReferencedObject {
public:
    virtual ~hkLocalFrameGroup() {}
};

// hkReferencedObjectLock - Forward declaration (defined after hkSingleton)
class hkReferencedObjectLock;

// hkRefNew - Forward declaration (defined after hkRefPtr)
template<typename T>
struct hkRefNew;

// hkRefPtr - Reference pointer with proper reference counting
// IDA: Various implementations at 0x1402f8200, 0x1402f8300, 0x1402fa4e0, 0x1402fa520, 0x1402fa5a0
template<typename T>
class hkRefPtr {
public:
    T* m_ptr;

    hkRefPtr() : m_ptr(nullptr) {}

    // IDA: ??0?$hkRefPtr@VhkaiNavMeshQueryMediator@@@@QEAA@PEAVhkaiNavMeshQueryMediator@@@Z @ 0x1402fa4e0
    hkRefPtr(T* e) : m_ptr(e) {
        if (e) {
            hkReferencedObject::addReference(reinterpret_cast<hkReferencedObject*>(e));
        }
    }

    // IDA: ??0?$hkRefPtr@VHavokNavMeshResource@@@@QEAA@AEBV0@@Z @ 0x1402fa5a0
    hkRefPtr(const hkRefPtr& rp) : m_ptr(rp.m_ptr) {
        if (m_ptr) {
            hkReferencedObject::addReference(reinterpret_cast<hkReferencedObject*>(m_ptr));
        }
    }

    // IDA: ??0?$hkRefPtr@VHavokNavMeshResource@@@@QEAA@V?$hkRefNew@VHavokNavMeshResource@@@@@Z @ 0x1402fa570
    // Constructor from hkRefNew - steals ownership without incrementing reference
    hkRefPtr(hkRefNew<T> rp) : m_ptr(rp.stealOwnership()) {}

    // IDA: ??1?$hkRefPtr@VhkStdioStreamReader@@@@QEAA@XZ @ 0x1402f7610
    ~hkRefPtr() {
        if (m_ptr) {
            hkReferencedObject::removeReference(reinterpret_cast<hkReferencedObject*>(m_ptr));
        }
    }

    // IDA: ??4?$hkRefPtr@VhkaiNavMeshQueryMediator@@@@QEAAXAEBV0@@Z @ 0x1402f8300
    hkRefPtr& operator=(const hkRefPtr& rp) {
        if (rp.m_ptr) {
            hkReferencedObject::addReference(reinterpret_cast<hkReferencedObject*>(rp.m_ptr));
        }
        if (m_ptr) {
            hkReferencedObject::removeReference(reinterpret_cast<hkReferencedObject*>(m_ptr));
        }
        m_ptr = rp.m_ptr;
        return *this;
    }

    // Assignment from raw pointer
    hkRefPtr& operator=(T* p) {
        if (p) {
            hkReferencedObject::addReference(reinterpret_cast<hkReferencedObject*>(p));
        }
        if (m_ptr) {
            hkReferencedObject::removeReference(reinterpret_cast<hkReferencedObject*>(m_ptr));
        }
        m_ptr = p;
        return *this;
    }

    // Assignment from nullptr
    hkRefPtr& operator=(std::nullptr_t) {
        if (m_ptr) {
            hkReferencedObject::removeReference(reinterpret_cast<hkReferencedObject*>(m_ptr));
        }
        m_ptr = nullptr;
        return *this;
    }

    // IDA: ??4?$hkRefPtr@VhkaiNavMeshInstance@@@@QEAAXV?$hkRefNew@VhkaiNavMeshInstance@@@@@Z @ 0x1402f8200
    hkRefPtr& operator=(hkRefNew<T> e) {
        if (m_ptr) {
            hkReferencedObject::removeReference(reinterpret_cast<hkReferencedObject*>(m_ptr));
        }
        m_ptr = e.stealOwnership();
        return *this;
    }

    // IDA: ?setAndDontIncrementRefCount@?$hkRefPtr@VhkaiNavMeshQueryMediator@@@@QEAAXPEAVhkaiNavMeshQueryMediator@@@Z @ 0x1402fa520
    void setAndDontIncrementRefCount(T* e) {
        if (m_ptr && m_ptr != e) {
            hkReferencedObject::removeReference(reinterpret_cast<hkReferencedObject*>(m_ptr));
        }
        m_ptr = e;
    }

    T* operator->() { return m_ptr; }
    const T* operator->() const { return m_ptr; }
    // operator* and const version disabled for void type to avoid reference to void error
    // T& operator*() { return *m_ptr; }
    // const T& operator*() const { return *m_ptr; }
    operator bool() const { return m_ptr != nullptr; }
};

// hkRefNew - Reference new helper (transfers ownership)
// IDA: ??$stealOwnership@VHavokNavMeshResource@@@@@ ?stealOwnership@?$hkRefNew@VHavokNavMeshResource@@@@QEAAPEAVHavokNavMeshResource@@XZ @ 0x1402fb620
template<typename T>
struct hkRefNew {
    T* m_pntr;

    hkRefNew(T* p) : m_pntr(p) {}

    // IDA: stealOwnership - returns pointer and clears m_pntr
    T* stealOwnership() {
        T* p = m_pntr;
        m_pntr = nullptr;
        return p;
    }
};

// hkTraitBool - Compile-time boolean trait
template<bool V>
struct hkTraitBool {
    static constexpr bool value = V;
};

// ============================================================================
// hkaiNavMeshSilhouetteSelector - Base class for silhouette selection
// IDA: Used as second base class in hkaiOverlapManager multiple inheritance
// IDA: ctor at ??0hkaiNavMeshSilhouetteSelector@@QEAA@VhkFinishLoadedObjectFlag@@@Z
// Simple vtable-only class, inherits from hkReferencedObject
// ============================================================================
class hkaiNavMeshSilhouetteSelector : public hkReferencedObject {
public:
    hkaiNavMeshSilhouetteSelector() : hkReferencedObject() {}

    hkaiNavMeshSilhouetteSelector(hkFinishLoadedObjectFlag flag) : hkReferencedObject() {
        (void)flag;
    }
};

// hkArrayBase - Havok array base class
// IDA: uses m_capacityAndFlags with bit 31 = external allocation flag
template<typename T>
class hkArrayBase {
public:
    T* m_data;
    int m_size;
    int m_capacityAndFlags;  // Upper bits used as flags, lower 30 bits = capacity

    // IDA: ??0?$hkArrayBase@I@@QEAA@XZ @ 0x1402f7870
    hkArrayBase() {
        m_data = nullptr;
        m_size = 0;
        m_capacityAndFlags = 0x80000000;  // External allocation flag set = no ownership
    }

    // IDA: ??A?$hkArrayBase@UPathPoint@hkaiPath@@@@QEAAAEAUPathPoint@hkaiPath@@H@Z @ 0x14026fec0
    T& operator[](int i) {
        return m_data[i];
    }

    // IDA: ??A?$hkArrayBase@I@@QEAAAEAIH@Z @ 0x140276ef0
    const T& operator[](int i) const {
        return m_data[i];
    }

    // IDA: ?getCapacity@?$hkArrayBase@UPathPoint@hkaiPath@@@@QEBAHXZ @ 0x140276fa0
    int getCapacity() const {
        return m_capacityAndFlags & 0x3FFFFFFF;  // Lower 30 bits
    }

    // IDA: ?isEmpty@?$hkArrayBase@UPathPoint@hkaiPath@@@@QEBA?AVhkBool@@XZ @ 0x14026fee0
    hkBool isEmpty() const {
        return hkBool(m_size == 0);
    }

    // IDA: ?clear@?$hkArrayBase@VhkVector4@@@@QEAAXXZ @ 0x1402f8260
    void clear() {
        m_size = 0;
    }

    // IDA: ?copy@?$hkArrayBase@VhkVector4@@@@SAXPEAVhkVector4@@PEBV2@H@Z @ 0x1402f8c10
    static void copy(T* dst, const T* src, int n) {
        for (int i = 0; i < n; ++i) {
            dst[i] = src[i];
        }
    }

    // IDA: ?copyFromArray@?$hkArrayBase@VhkVector4@@@@IEAAAEAV1@AEAVhkMemoryAllocator@@AEBV1@U?$hkTraitBool@$00@@@Z @ 0x1402f8c80
    template<bool HasTrivialCopy>
    hkArrayBase& copyFromArray(hkMemoryAllocator* alloc, const hkArrayBase& other, hkTraitBool<HasTrivialCopy>) {
        int cap = getCapacity();
        if (cap < other.m_size) {
            // Need to reallocate
            if (m_capacityAndFlags >= 0) {
                alloc->bufFree<T>(m_data, cap);
            }
            int n = other.m_size;
            m_data = alloc->bufAlloc<T>(&n);
            m_capacityAndFlags = n;
        }
        m_size = other.m_size;
        copy(m_data, other.m_data, m_size);
        return *this;
    }

    // IDA: ?_clearAndDeallocate@?$hkArrayBase@UPathPoint@hkaiPath@@@@QEAAXAEAVhkMemoryAllocator@@@Z @ 0x140276d40
    void _clearAndDeallocate(hkMemoryAllocator* alloc) {
        clear();
        if (m_capacityAndFlags >= 0) {  // Not externally allocated
            int cap = getCapacity();
            alloc->bufFree<T>(m_data, cap);
        }
        m_data = nullptr;
        m_capacityAndFlags = 0x80000000;
    }

    // IDA: ?_append@?$hkArrayBase@VhkVector4@@@@QEAAXAEAVhkMemoryAllocator@@PEBVhkVector4@@H@Z @ 0x14079e610
    void _append(hkMemoryAllocator* alloc, const T* elements, int numToInsert) {
        int newSize = m_size + numToInsert;
        int capacity = getCapacity();
        if (capacity < newSize) {
            // Need to reserve more space
            int newCapacity = capacity * 2;
            if (newCapacity < newSize) {
                newCapacity = newSize;
            }
            hkArrayUtil::_reserve(*alloc, reinterpret_cast<void**>(this), newCapacity, sizeof(T));
        }
        // Copy elements to end
        T* dst = &m_data[m_size];
        for (int i = 0; i < numToInsert; ++i) {
            dst[i] = elements[i];
        }
        m_size = newSize;
    }

    // IDA: ?_spliceInto@?$hkArrayBase@D@@QEAAXAEAVhkMemoryAllocator@@HHPEBDH@Z @ 0x1407a7600
    void _spliceInto(hkMemoryAllocator* alloc, int index, int numDel, const T* p, int numToInsert) {
        int capacity = getCapacity();
        int newSize = m_size + numToInsert - numDel;
        int numToMove = m_size - index - numDel;

        if (capacity < newSize) {
            // Need to reserve more space
            int newCapacity = capacity * 2;
            if (newCapacity < newSize) {
                newCapacity = newSize;
            }
            hkArrayUtil::_reserve(*alloc, reinterpret_cast<void**>(this), newCapacity, sizeof(T));
        }

        // Move existing elements to make room
        if (numToMove > 0) {
            hkMemUtil::memMove(&m_data[index + numToInsert], &m_data[index + numDel], numToMove * sizeof(T));
        }

        // Copy new elements
        for (int i = 0; i < numToInsert; ++i) {
            m_data[index + i] = p[i];
        }
        m_size = newSize;
    }

    // IDA: ?_insertAt@?$hkArrayBase@D@@QEAAXAEAVhkMemoryAllocator@@HPEBDH@Z @ 0x1407a76d0
    void _insertAt(hkMemoryAllocator* alloc, int index, const T* p, int numToInsert) {
        _spliceInto(alloc, index, 0, p, numToInsert);
    }
};

// hkArrayUtil - Array utility functions
// IDA: Functions at 0x14078f0c0, 0x14078f170, 0x14078f210
namespace hkArrayUtil {
    // IDA: ??$destruct@VhkVector4@@@hkArrayUtil@@SAXPEAVhkVector4@@HU?$hkTraitBool@$00@@@Z @ 0x1402f8ba0
    template<typename T>
    void destruct(T* data, int size, bool hasDestructor) {
        // Stub - destruct elements if hasDestructor is true
    }

    // IDA: ??$constructWithCopy@VhkVector4@@@hkArrayUtil@@SAXPEAVhkVector4@@HAEBV1@U?$hkTraitBool@$00@@@Z @ 0x1402f7690
    template<typename T>
    void constructWithCopy(T* dst, int count, const T& src, bool hasCopyConstructor) {
        for (int i = 0; i < count; ++i) {
            new (&dst[i]) T(src);
        }
    }

    // IDA: ?_reserve@hkArrayUtil@@SA?AUhkResult@@AEAVhkMemoryAllocator@@PEAXHH@Z @ 0x14078f0c0
    // Reserve capacity for array elements
    // array pointer layout: [0] = data ptr, [1] = ?, [2] = size, [3] = capacity (negative for external)
    inline hkResult _reserve(hkMemoryAllocator& mem, void** array, int numElem, int sizeElem) {
        int oldSize = sizeElem * reinterpret_cast<int*>(array)[2];  // m_size
        int capacity = reinterpret_cast<int*>(array)[3];  // m_capacityAndFlags

        int allocSize = sizeElem * numElem;

        void* newData;
        if (capacity < 0) {
            // External buffer - allocate new and copy
            newData = mem.bufAlloc(allocSize);
            if (newData) {
                memcpy(newData, *array, oldSize);
            }
        } else {
            // Internal buffer - realloc
            newData = mem.bufRealloc(*array, sizeElem * (capacity & 0x3FFFFFFF), &allocSize);
        }

        *array = newData;
        reinterpret_cast<int*>(array)[3] = allocSize / sizeElem;

        hkResult result;
        result.m_result = (newData == nullptr) ? 1 : 0;
        return result;
    }

    // Overload taking hkResult pointer (IDA calling convention)
    inline void _reserve(hkResult* result, hkMemoryAllocator& mem, void** array, int numElem, int sizeElem) {
        hkResult res = _reserve(mem, array, numElem, sizeElem);
        if (result) *result = res;
    }

    // IDA: ?_reserveMore@hkArrayUtil@@SAXAEAVhkMemoryAllocator@@PEAXH@Z @ 0x14078f170
    // Double the array capacity (or set to 1 if empty)
    inline void _reserveMore(hkMemoryAllocator& mem, void** array, int sizeElem) {
        int size = reinterpret_cast<int*>(array)[2];  // m_size
        int newCapacity = size ? size * 2 : 1;
        int capacity = reinterpret_cast<int*>(array)[3];  // m_capacityAndFlags

        int reqNumBytes = sizeElem * newCapacity;

        void* newData;
        if (capacity < 0) {
            // External buffer - allocate new and copy
            newData = mem.bufAlloc(reqNumBytes);
            memcpy(newData, *array, sizeElem * size);
            *array = newData;
        } else {
            // Internal buffer - realloc
            *array = mem.bufRealloc(*array, sizeElem * (capacity & 0x3FFFFFFF), &reqNumBytes);
        }

        reinterpret_cast<int*>(array)[3] = reqNumBytes / sizeElem;
    }

    // IDA: ?_reduce@hkArrayUtil@@SAXAEAVhkMemoryAllocator@@PEAXHPEADH@Z @ 0x14078f210
    // Reduce array capacity, possibly to in-place memory
    inline void _reduce(hkMemoryAllocator& mem, void** array, int sizeElem, void* inplaceMem, int requestedCapacity) {
        int size = reinterpret_cast<int*>(array)[2];  // m_size
        int capacity = reinterpret_cast<int*>(array)[3];  // m_capacityAndFlags

        if (inplaceMem && size < requestedCapacity) {
            // Copy to in-place memory
            memcpy(inplaceMem, *array, sizeElem * size);
            mem.blockFree(*array, sizeElem * (capacity & 0x3FFFFFFF));
            reinterpret_cast<int*>(array)[3] = requestedCapacity | 0x80000000;
            *array = inplaceMem;
        } else {
            // Reallocate to smaller size
            int reqNumBytes = requestedCapacity * sizeElem;
            *array = mem.bufRealloc(*array, sizeElem * capacity, &reqNumBytes);
            reinterpret_cast<int*>(array)[3] = reqNumBytes / sizeElem;
        }
    }
}

// hkArray - Havok array (std::vector wrapper)
template<typename T, typename Allocator = hkContainerHeapAllocator>
class hkArray : public hkArrayBase<T> {
public:
    // IDA: ??0?$hkArray@IUhkContainerHeapAllocator@@@@QEAA@XZ @ 0x140276c90
    hkArray() : hkArrayBase<T>() {}

    // IDA: ?clearAndDeallocate@?$hkArray@UPathPoint@hkaiPath@@UhkContainerHeapAllocator@@@@QEAAXXZ @ 0x140276db0
    void clearAndDeallocate() {
        Allocator alloc;
        this->_clearAndDeallocate(&alloc.get());
    }

    // IDA: ??1?$hkArray@UPathPoint@hkaiPath@@UhkContainerHeapAllocator@@@@QEAA@XZ @ 0x140276df0
    ~hkArray() {
        clearAndDeallocate();
    }

    // IDA: ?pushBack@?$hkArray@VhkVector4@@UhkContainerHeapAllocator@@@@QEAAXAEBVhkVector4@@@Z @ 0x1402f74f0
    void pushBack(const T& e) {
        Allocator alloc;
        _pushBack(&alloc.get(), e);
    }

    // IDA: ?_pushBack@?$hkArrayBase@VhkVector4@@@@QEAAXAEAVhkMemoryAllocator@@AEBVhkVector4@@@Z @ 0x1402f7540
    void _pushBack(hkMemoryAllocator* alloc, const T& t) {
        if (this->m_size == this->getCapacity()) {
            hkArrayUtil::_reserveMore(*alloc, reinterpret_cast<void**>(this), sizeof(T));
        }
        hkArrayUtil::constructWithCopy(&this->m_data[this->m_size], 1, t, true);
        ++this->m_size;
    }

    // size() - returns number of elements
    size_t size() const {
        return static_cast<size_t>(this->m_size);
    }

    // empty() - checks if array is empty
    bool empty() const {
        return this->m_size == 0;
    }

    // IDA: ??4?$hkArray@VhkVector4@@UhkContainerHeapAllocator@@@@QEAAAEAV0@AEBV?$hkArrayBase@VhkVector4@@@@@Z @ 0x1402f8d60
    // operator= - assign from hkArrayBase
    hkArray& operator=(const hkArrayBase<T>& other) {
        Allocator alloc;
        this->copyFromArray(&alloc.get(), &other, hkTraitBool<true>());
        return *this;
    }

    // resize() - resize array to new size
    void resize(size_t newSize) {
        // Simple stub - just set size (actual implementation would allocate/reallocate)
        this->m_size = static_cast<int>(newSize);
    }

    static bool isEmpty(const hkArray* arr) {
        return arr->m_size == 0;
    }

    static T* get(hkArray* arr, size_t idx) {
        return &arr->m_data[idx];
    }
};

// ============================================================================
// hkaiOverlapManager - Manages overlap detection for Havok AI navigation
// IDA: ctor ??0hkaiOverlapManager@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1408103f0
// Multiple inheritance: hkReferencedObject (offset 0) + hkaiNavMeshSilhouetteSelector (offset 8)
// Note: hkaiNavMeshSilhouetteSelector already inherits hkReferencedObject, so only one vtable chain
// Layout: [0x00] vtable for hkReferencedObject, [0x08] vtable for hkaiNavMeshSilhouetteSelector,
//         [0x10] m_silhouetteFilter (hkRefPtr, 8 bytes), [0x18+] m_sections (hkArray<Section>)
// ============================================================================
class hkaiOverlapManager : public hkaiNavMeshSilhouetteSelector {
public:
    // hkRefPtr<void> m_silhouetteFilter at offset 0x10 (after hkReferencedObject base at 0x00)
    hkRefPtr<void> m_silhouetteFilter;

    // Section struct for per-section overlap data
    // IDA: ?rebuildMap@Section@hkaiOverlapManager@@QEAAX_N@Z @ 0x14080fc30
    struct Section {
        // +0: hkMapBase pointer (tree map for fast lookup)
        void* m_treeMap;
        // +8: pointer to array of item pointers (items to insert into map)
        void** m_itemsPtr;
        // +16: count of items to process
        int m_itemCount;
        // +20 padding
        int _pad20;
        // +24: m_data pointer (hkArray of 16-byte elements)
        void* m_dataPtr;
        // +32: m_data.m_size
        int m_dataSize;
        // +36: m_data.m_capacityAndFlags
        int m_dataCapacityAndFlags;
        // +40-55: padding to reach +56
        char _pad40[16];
        // +56: total m_size
        int m_totalSize;
        // +60-63: padding
        int _pad60;

        Section() : m_treeMap(nullptr), m_itemsPtr(nullptr), m_itemCount(0), _pad20(0),
                    m_dataPtr(nullptr), m_dataSize(0), m_dataCapacityAndFlags(0x80000000),
                    m_totalSize(0), _pad60(0) {}
    };

    // hkArray<Section> m_sections at offset 0x18
    hkArray<Section, hkContainerHeapAllocator> m_sections;

    hkaiOverlapManager() : hkaiNavMeshSilhouetteSelector(), m_silhouetteFilter() {}

    hkaiOverlapManager(hkFinishLoadedObjectFlag flag);

    // IDA: ?rebuildMaps@hkaiOverlapManager@@IEAAX_N@Z @ 0x14080ffa0
    void rebuildMaps(bool updateContexts);

private:
    // IDA: ?rebuildMap@Section@hkaiOverlapManager@@QEAAX_N@Z @ 0x14080fc30
    static void sectionRebuildMap(Section* section, bool updateContexts);
};

// hkMapOperations - Default operations for map keys
template<typename Key>
struct hkMapOperations {
    static unsigned int hash(const Key& key) {
        // Simple hash: multiply by a prime and shift
        return static_cast<unsigned int>(-1640531535 * (static_cast<unsigned long long>(key) >> 4));
    }
    static bool equal(const Key& a, const Key& b) {
        return a == b;
    }
};

// hkMapBase - Open addressing hash table with linear probing
// IDA: ?insert@?$hkMapBase@_K_KU?$hkMapOperations@_K@@@@QEAAIAEAVhkMemoryAllocator@@_K1@Z @ 0x1407a8fa0
template<typename Key, typename Value, typename Ops = hkMapOperations<Key>>
class hkMapBase {
public:
    struct Pair {
        Key key;
        Value val;
    };

    Pair* m_elem;        // Element array
    int m_numElems;      // Number of elements
    int m_hashMod;       // Hash mask (capacity - 1)

    // IDA: ??0?$hkMapBase@_K_KU?$hkMapOperations@_K@@@@QEAA@PEAXH@Z @ 0x1407a9410
    hkMapBase(void* mem, int capacity) {
        m_elem = static_cast<Pair*>(mem);
        m_numElems = 0;
        m_hashMod = capacity - 1;
        // Initialize all keys to -1 (empty marker)
        for (int i = 0; i <= m_hashMod; ++i) {
            m_elem[i].key = static_cast<Key>(-1);
        }
    }

    // IDA: ?init@?$hkMapBase@_K_KU?$hkMapOperations@_K@@@@QEAAXPEAXH@Z @ 0x1407a92c0
    void init(void* mem, int capacity) {
        m_elem = static_cast<Pair*>(mem);
        m_numElems = 0;
        m_hashMod = capacity - 1;
        for (int i = 0; i <= m_hashMod; ++i) {
            m_elem[i].key = static_cast<Key>(-1);
        }
    }

    // IDA: ?getSizeInBytesFor@?$hkMapBase@_K_KU?$hkMapOperations@_K@@@@SAHH@Z @ 0x1407a8f70
    static int getSizeInBytesFor(int capacity) {
        return capacity * sizeof(Pair);
    }

    // IDA: ?insert@?$hkMapBase@_K_KU?$hkMapOperations@_K@@@@QEAAIAEAVhkMemoryAllocator@@_K1@Z @ 0x1407a8fa0
    // Returns 1 if new key inserted, 0 if existing key updated
    int insert(hkMemoryAllocator* alloc, Key key, Value val) {
        // Resize if load factor > 0.5
        if (2 * m_numElems > m_hashMod) {
            resizeTable(alloc, 2 * m_hashMod + 2);
        }

        unsigned int hash = Ops::hash(key);
        unsigned int idx = m_hashMod & hash;

        bool isNew = true;
        // Linear probing
        while (m_elem[idx].key != static_cast<Key>(-1)) {
            if (m_elem[idx].key == key) {
                isNew = false;
                break;
            }
            idx = m_hashMod & (idx + 1);
        }

        m_numElems += isNew ? 1 : 0;
        m_elem[idx].key = key;
        m_elem[idx].val = val;
        return isNew ? 1 : 0;
    }

    // IDA: ?findKey@?$hkMapBase@_K_KU?$hkMapOperations@_K@@@@QEBAPEAVDummy@@_K@Z @ 0x1407a9050
    // Returns index as Dummy*, or (m_hashMod+1) if not found
    void* findKey(Key key) const {
        if (m_hashMod <= 0) {
            return reinterpret_cast<void*>(static_cast<uintptr_t>(m_hashMod + 1));
        }

        unsigned int hash = Ops::hash(key);
        unsigned int idx = m_hashMod & hash;

        while (m_elem[idx].key != static_cast<Key>(-1)) {
            if (m_elem[idx].key == key) {
                return reinterpret_cast<void*>(static_cast<uintptr_t>(idx));
            }
            idx = m_hashMod & (idx + 1);
        }
        return reinterpret_cast<void*>(static_cast<uintptr_t>(m_hashMod + 1));
    }

    // IDA: ?getWithDefault@?$hkMapBase@_K_KU?$hkMapOperations@_K@@@@QEBA_K_K0@Z @ 0x1407a90b0
    Value getWithDefault(Key key, Value defaultVal) const {
        void* idx = findKey(key);
        int index = static_cast<int>(reinterpret_cast<uintptr_t>(idx));
        if (index > m_hashMod) {
            return defaultVal;
        }
        return m_elem[index].val;
    }

    // IDA: ?get@?$hkMapBase@_K_KU?$hkMapOperations@_K@@@@QEBA?AUhkResult@@_KPEA_K@Z @ 0x1407a9110
    hkResult get(Key key, Value* outVal) const {
        void* idx = findKey(key);
        int index = static_cast<int>(reinterpret_cast<uintptr_t>(idx));
        hkResult result;
        if (index > m_hashMod) {
            result.m_result = 1;  // Not found
        } else {
            *outVal = m_elem[index].val;
            result.m_result = 0;  // Success
        }
        return result;
    }

    // IDA: ?clearAndDeallocate@?$hkMapBase@_K_KU?$hkMapOperations@_K@@@@QEAAXAEAVhkMemoryAllocator@@@Z @ 0x1407a9270
    void clearAndDeallocate(hkMemoryAllocator* alloc) {
        if (m_elem && m_hashMod >= 0) {
            alloc->bufFree(m_elem, (m_hashMod + 1) * sizeof(Pair));
        }
        m_elem = nullptr;
        m_numElems = 0;
        m_hashMod = -1;
    }

    // IDA: ?resizeTable@?$hkMapBase@_K_KU?$hkMapOperations@_K@@@@IEAA?AUhkResult@@AEAVhkMemoryAllocator@@H@Z @ 0x1407a9300
    hkResult resizeTable(hkMemoryAllocator* alloc, int newCapacity) {
        hkResult result;
        Pair* oldElem = m_elem;
        int oldCapacity = m_hashMod + 1;

        int numBytes = newCapacity * sizeof(Pair);
        m_elem = static_cast<Pair*>(alloc->bufAlloc(numBytes));
        if (!m_elem) {
            m_elem = oldElem;
            result.m_result = 1;
            return result;
        }

        m_hashMod = newCapacity - 1;
        m_numElems = 0;

        // Initialize new table
        for (int i = 0; i < newCapacity; ++i) {
            m_elem[i].key = static_cast<Key>(-1);
        }

        // Rehash old elements
        if (oldElem) {
            for (int i = 0; i < oldCapacity; ++i) {
                if (oldElem[i].key != static_cast<Key>(-1)) {
                    insert(alloc, oldElem[i].key, oldElem[i].val);
                }
            }
            alloc->bufFree(oldElem, oldCapacity * sizeof(Pair));
        }

        result.m_result = 0;
        return result;
    }

    // IDA: ?tryInsert@?$hkMapBase@_K_KU?$hkMapOperations@_K@@@@QEAAIAEAVhkMemoryAllocator@@_K1AEAUhkResult@@@Z @ 0x1407a9430
    int tryInsert(hkMemoryAllocator* alloc, Key key, Value val, hkResult* result) {
        // Check if key already exists
        void* idx = findKey(key);
        int index = static_cast<int>(reinterpret_cast<uintptr_t>(idx));
        if (index <= m_hashMod) {
            // Key exists, don't insert
            result->m_result = 1;
            return 0;
        }
        // Insert new key
        *result = resizeTable(alloc, m_hashMod + 1);  // Ensure capacity
        return insert(alloc, key, val);
    }

    // IDA: ?remove@?$hkMapBase@_K_KU?$hkMapOperations@_K@@@@QEAA?AUhkResult@@_K@Z @ 0x1407a9580
    hkResult remove(Key key) {
        void* idx = findKey(key);
        int index = static_cast<int>(reinterpret_cast<uintptr_t>(idx));
        hkResult result;
        if (index > m_hashMod) {
            result.m_result = 1;  // Not found
            return result;
        }

        // Mark as deleted
        m_elem[index].key = static_cast<Key>(-1);
        --m_numElems;

        // Rehash following elements
        index = m_hashMod & (index + 1);
        while (m_elem[index].key != static_cast<Key>(-1)) {
            Key k = m_elem[index].key;
            Value v = m_elem[index].val;
            m_elem[index].key = static_cast<Key>(-1);
            --m_numElems;
            insert(nullptr, k, v);  // Re-insert without allocation
            index = m_hashMod & (index + 1);
        }

        result.m_result = 0;
        return result;
    }

    // IDA: ?reserve@?$hkMapBase@_K_KU?$hkMapOperations@_K@@@@QEAAXAEAVhkMemoryAllocator@@H@Z @ 0x1407a95d0
    void reserve(hkMemoryAllocator* alloc, int capacity) {
        if (capacity > m_hashMod + 1) {
            resizeTable(alloc, capacity);
        }
    }
};

// ============================================================================
// Helper functions from IDA
// ============================================================================

// IDA: getRandomPoints - generate random points for silhouette
inline void getRandomPoints(hkPseudoRandomGenerator* prg, const hkVector4* bounds, void* points) {
    // Stub - generate random points
}

// ============================================================================
// External symbols from IDA
// ============================================================================

// IDA: FLOAT_N0_41652954, FLOAT_N9_8828163, FLOAT_0_88999999
namespace HavokConstants {
    constexpr float FLOAT_N0_41652954 = -0.41652954f;
    constexpr float FLOAT_N9_8828163 = -9.8828163f;
    constexpr float FLOAT_0_88999999 = 0.88999999f;
}

// ============================================================================
// Inline implementations that require complete types
// ============================================================================

// IDA: ?setInstanceAndTransform@QueryInputBase@hkaiNavMeshQueryMediator@@QEAAXPEBVhkaiNavMeshInstance@@@Z @ 0x140280e70
inline void hkaiNavMeshQueryMediator::QueryInputBase::setInstanceAndTransform(const hkaiNavMeshInstance* instance) {
    m_instance = instance;
    if (instance) {
        m_localToWorldTransform = &instance->getTransform();
    } else {
        m_localToWorldTransform = nullptr;
    }
}

// hkaiPathfindingUtil - Pathfinding utilities (defined after hkArray)
namespace hkaiPathfindingUtil {

    enum SearchStatus {
        SEARCH_SUCCEEDED = 0,
        SEARCH_FAILED = 1
    };

    struct FindPathInput {
        hkVector4 m_startPoint;
        unsigned int m_startFaceKey;
        hkArray<hkVector4> m_goalPoints;
        hkArray<unsigned int> m_goalFaceKeys;

        struct AgentInfo {
            float m_diameter;
        } m_agentInfo;

        struct SearchParameters {
            hkVector4 m_up;
        } m_searchParameters;

        FindPathInput() : m_startFaceKey(0) {}

        static void Init(FindPathInput* input, int) {
            new (input) FindPathInput();
        }

        // IDA: ??1FindPathInput@hkaiPathfindingUtil@@UEAA@XZ @ 0x1402771d0
        virtual ~FindPathInput() {}
    };

    // FindPathOutput - Pathfinding output with proper constructor/destructor
    // IDA: ??0FindPathOutput@hkaiPathfindingUtil@@QEAA@XZ @ 0x140276e90
    // IDA: ??1FindPathOutput@hkaiPathfindingUtil@@UEAA@XZ @ 0x140276c00
    struct FindPathOutput : public hkReferencedObject {
        hkArray<hkaiPath::PathPoint> m_pathOut;
        hkArray<unsigned int> m_visitedEdges;
        hkaiAstarOutputParameters m_outputParameters;

        FindPathOutput() : hkReferencedObject() {
            // Arrays are initialized by their default constructors
        }

        // IDA: ??_GFindPathOutput@hkaiPathfindingUtil@@UEAAPEAXI@Z @ 0x140276c50
        virtual ~FindPathOutput() {
            // Arrays destroyed by their destructors
        }

        static void Init(FindPathOutput* output) {
            new (output) FindPathOutput();
        }

        static void Destroy(FindPathOutput* output) {
            output->~FindPathOutput();
        }
    };

    static void findPath(const void* collection) {
        // Stub - actual Havok pathfinding call
    }
}

// hkaiSingleCharacterBehavior - Single character behavior base class
// Defined after hkaiPathfindingUtil and hkRefPtr/hkArray
class hkaiSingleCharacterBehavior {
public:
    virtual ~hkaiSingleCharacterBehavior() {}

    // IDA: ?getNumCharacters@hkaiSingleCharacterBehavior@@UEBAHXZ @ 0x1402a3d40
    virtual int getNumCharacters() const { return 1; }
};

// hkaiPathFollowingBehavior - Path following behavior
// IDA: ??1hkaiPathFollowingBehavior@@UEAA@XZ @ 0x1402f7480
class hkaiPathFollowingBehavior : public hkaiSingleCharacterBehavior {
public:
    struct RequestedGoalPoint {
        hkVector4 m_position;
        int m_faceKey;
    };

    hkRefPtr<void> m_currentPathFixed;
    hkRefPtr<void> m_currentPath;
    hkArray<RequestedGoalPoint> m_requestedGoalPoints;
    hkRefPtr<void> m_pathFollowingProperties;

    hkaiPathFollowingBehavior() : hkaiSingleCharacterBehavior() {}

    // IDA: ??1hkaiPathFollowingBehavior@@UEAA@XZ @ 0x1402f7480
    virtual ~hkaiPathFollowingBehavior() {
        // Destructor calls hkRefPtr destructors and hkArray destructor
        // Then calls parent destructor
    }

    // IDA: ?getNumCharacters@hkaiSingleCharacterBehavior@@UEBAHXZ - inherited, returns 1
};

// hkaiWorld_Cinfo - World configuration
// IDA: ??1Cinfo@hkaiWorld@@QEAA@XZ @ 0x1402f8010
struct hkaiWorld_Cinfo {
    hkaiPathfindingUtil::FindPathInput m_pathfindingInput;
    // Additional members for volume pathfinding
    char m_volumeData[440]; // Remaining bytes to total 464

    hkaiWorld_Cinfo() {
        memset(m_volumeData, 0, sizeof(m_volumeData));
    }

    ~hkaiWorld_Cinfo() {
        // IDA: Calls FindPathInput destructors
    }

    static void Init(hkaiWorld_Cinfo* c) {
        new (c) hkaiWorld_Cinfo();
    }

    static void Destroy(hkaiWorld_Cinfo* c) {
        c->~hkaiWorld_Cinfo();
    }
};

// hkaiVolumePathfindingUtil - Volume pathfinding utilities
namespace hkaiVolumePathfindingUtil {

    struct FindPathInput : public hkReferencedObject {
        hkArray<hkVector4> m_goalPoints;
        hkArray<unsigned int> m_goalCellKeys;
        // Additional agent info
        char m_agentInfo[16];

        FindPathInput() : hkReferencedObject() {}

        virtual ~FindPathInput() {
            // IDA: ??1FindPathInput@hkaiVolumePathfindingUtil@@UEAA@XZ @ 0x1402f84a0
            // Destroys arrays and calls parent destructor
        }
    };
}

// hkClassEnum - Havok class enum reflection
// IDA: Used by hkClass::getEnum, getEnumByName
struct hkClassEnum {
    const char* m_name;
    // Additional members would be here (items, values, etc.)
    // For now, minimal definition for getEnumByName
};

// hkClassMember - Havok class member reflection
// IDA: Used by hkClass::getMember, getDeclaredMember
struct hkClassMember {
    const char* m_name;
    // Additional members would be here (type, offset, etc.)
    // For now, minimal definition
};

// hkCustomAttributes - Custom attributes container (forward declaration)
class hkCustomAttributes;

// hkVariant - Variant type for attributes
struct hkVariant {
    int m_type;
    void* m_data;
};

// Forward declaration
class hkClass;

// hkClassFlags - Flags type for hkClass
enum class hkClassFlagValues : unsigned int {
    None = 0,
    // Additional flag values would be defined here
};
using hkClassFlags = hkFlags<hkClassFlagValues, unsigned int>;

// hkClass - Havok class reflection
// IDA: Multiple functions at 0x140790xxx
class hkClass {
public:
    // Flag values enum (nested)
    enum FlagValues : unsigned int {
        FLAGS_NONE = 0,
        // Additional flag values
    };

    const char* m_name;
    const hkClass* m_parent;
    int m_objectSize;
    const hkClass** m_implementedInterfaces;
    int m_numImplementedInterfaces;
    const hkClassEnum* m_declaredEnums;
    int m_numDeclaredEnums;
    const hkClassMember* m_declaredMembers;
    int m_numDeclaredMembers;
    const void* m_defaults;
    const hkCustomAttributes* m_attributes;
    hkFlags<FlagValues, unsigned int> m_flags;
    int m_describedVersion;

    // Default constructor
    hkClass() : m_name(nullptr), m_parent(nullptr), m_objectSize(0),
                m_implementedInterfaces(nullptr), m_numImplementedInterfaces(0),
                m_declaredEnums(nullptr), m_numDeclaredEnums(0),
                m_declaredMembers(nullptr), m_numDeclaredMembers(0),
                m_defaults(nullptr), m_attributes(nullptr), m_describedVersion(0) {}

    // Simple name-only constructor
    hkClass(const char* name) : m_name(name), m_parent(nullptr), m_objectSize(0),
                m_implementedInterfaces(nullptr), m_numImplementedInterfaces(0),
                m_declaredEnums(nullptr), m_numDeclaredEnums(0),
                m_declaredMembers(nullptr), m_numDeclaredMembers(0),
                m_defaults(nullptr), m_attributes(nullptr), m_describedVersion(0) {}

    // Full constructor (IDA: 0x140790cb0)
    hkClass(const char* className, const hkClass* parentClass, int objectSizeInBytes,
            const hkClass** implementedInterfaces, int numImplementedInterfaces,
            const hkClassEnum* declaredEnums, int numDeclaredEnums,
            const hkClassMember* members, int numMembers,
            const void* defaults, const hkCustomAttributes* attrs,
            unsigned int flags, unsigned int version)
        : m_name(className), m_parent(parentClass), m_objectSize(objectSizeInBytes),
          m_implementedInterfaces(implementedInterfaces), m_numImplementedInterfaces(numImplementedInterfaces),
          m_declaredEnums(declaredEnums), m_numDeclaredEnums(numDeclaredEnums),
          m_declaredMembers(members), m_numDeclaredMembers(numMembers),
          m_defaults(defaults), m_attributes(attrs), m_flags(flags), m_describedVersion(version) {}

    // IDA: ?getName@hkClass@@QEBAPEBDXZ @ 0x1407903c0
    const char* getName() const { return m_name; }

    // IDA: ?equals@hkClass@@QEBA_NPEBV1@@Z @ 0x1407903d0
    bool equals(const hkClass* other) const {
        if (!other) {
            return false;
        }
        return hkString::strCmp(this->getName(), other->getName()) == 0;
    }

    // IDA: ?getParent@hkClass@@QEBAPEBV1@XZ @ 0x140790410
    const hkClass* getParent() const { return m_parent; }

    // IDA: ?getParent@hkClass@@QEAAPEAV1@XZ @ 0x140790420
    hkClass* getParent() { return const_cast<hkClass*>(m_parent); }

    // IDA: ?getInheritanceDepth@hkClass@@QEBAHXZ @ 0x140790430
    int getInheritanceDepth() const {
        int depth = 0;
        for (const hkClass* cls = this; cls != nullptr; cls = cls->m_parent) {
            depth++;
        }
        return depth;
    }

    // IDA: ?isSuperClass@hkClass@@QEBA?AVhkBool@@AEBV1@@Z @ 0x140790460
    hkBool isSuperClass(const hkClass* k) const {
        if (!k) {
            return hkBool(false);
        }
        for (const hkClass* parent = k; parent != nullptr; parent = parent->m_parent) {
            if (hkString::strCmp(this->getName(), parent->getName()) == 0) {
                return hkBool(true);
            }
        }
        return hkBool(false);
    }

    // IDA: ?getNumInterfaces@hkClass@@QEBAHXZ @ 0x1407904f0
    int getNumInterfaces() const {
        int count = m_numImplementedInterfaces;
        for (const hkClass* p = m_parent; p != nullptr; p = p->m_parent) {
            count += p->m_numImplementedInterfaces;
        }
        return count;
    }

    // IDA: ?getInterface@hkClass@@QEBAPEBV1@H@Z @ 0x140790520
    const hkClass* getInterface(int i) const {
        // Stub - requires full interface implementation
        return nullptr;
    }

    // IDA: ?getDeclaredInterface@hkClass@@QEBAPEBV1@H@Z @ 0x140790530
    const hkClass* getDeclaredInterface(int i) const {
        // Stub - requires full interface implementation
        return nullptr;
    }

    // IDA: ?getNumDeclaredInterfaces@hkClass@@QEBAHXZ @ 0x140790540
    int getNumDeclaredInterfaces() const { return m_numImplementedInterfaces; }

    // IDA: ?getNumEnums@hkClass@@QEBAHXZ @ 0x140790550
    int getNumEnums() const {
        int count = m_numDeclaredEnums;
        for (const hkClass* p = m_parent; p != nullptr; p = p->m_parent) {
            count += p->m_numDeclaredEnums;
        }
        return count;
    }

    // IDA: ?getEnum@hkClass@@QEBAAEBVhkClassEnum@@H@Z @ 0x140790580
    const hkClassEnum* getEnum(int enumIndex) const;

    // IDA: ?getEnumByName@hkClass@@QEBAPEBVhkClassEnum@@PEBD@Z @ 0x1407905d0
    const hkClassEnum* getEnumByName(const char* name) const;

    // IDA: ?getDeclaredEnum@hkClass@@QEBAAEBVhkClassEnum@@H@Z @ 0x140790650
    const hkClassEnum* getDeclaredEnum(int enumIndex) const {
        return &m_declaredEnums[enumIndex];
    }

    // IDA: ?getNumDeclaredEnums@hkClass@@QEBAHXZ @ 0x140790670
    int getNumDeclaredEnums() const { return m_numDeclaredEnums; }

    // IDA: ?getNumMembers@hkClass@@QEBAHXZ @ 0x140790680
    int getNumMembers() const {
        int count = m_numDeclaredMembers;
        for (const hkClass* p = m_parent; p != nullptr; p = p->m_parent) {
            count += p->m_numDeclaredMembers;
        }
        return count;
    }

    // IDA: ?getMember@hkClass@@QEBAAEBVhkClassMember@@H@Z @ 0x1407906b0
    const hkClassMember* getMember(int memberIndex) const;

    // IDA: ?getMember@hkClass@@QEAAAEAVhkClassMember@@H@Z @ 0x140790700
    hkClassMember* getMember(int memberIndex) {
        return const_cast<hkClassMember*>(static_cast<const hkClass*>(this)->getMember(memberIndex));
    }

    // IDA: ?getNumDeclaredMembers@hkClass@@QEBAHXZ @ 0x140790710
    int getNumDeclaredMembers() const { return m_numDeclaredMembers; }

    // IDA: ?getDeclaredMember@hkClass@@QEBAAEBVhkClassMember@@H@Z @ 0x140790720
    const hkClassMember* getDeclaredMember(int i) const {
        return &m_declaredMembers[i];
    }

    // IDA: ?getDeclaredMemberByName@hkClass@@QEBAPEBVhkClassMember@@PEBD@Z @ 0x140790740
    const hkClassMember* getDeclaredMemberByName(const char* name) const {
        int numMembers = getNumDeclaredMembers();
        for (int i = 0; i < numMembers; i++) {
            const hkClassMember* member = getDeclaredMember(i);
            if (hkString::strCmp(member->m_name, name) == 0) {
                return member;
            }
        }
        return nullptr;
    }

    // IDA: ?getMemberByName@hkClass@@QEBAPEBVhkClassMember@@PEBD@Z @ 0x1407907c0
    const hkClassMember* getMemberByName(const char* name) const {
        int numMembers = getNumMembers();
        for (int i = 0; i < numMembers; i++) {
            const hkClassMember* member = getMember(i);
            if (hkString::strCmp(member->m_name, name) == 0) {
                return member;
            }
        }
        return nullptr;
    }

    // IDA: ?getMemberIndexByName@hkClass@@QEBAHPEBD@Z @ 0x140790840
    int getMemberIndexByName(const char* name) const {
        int numMembers = getNumMembers();
        for (int i = 0; i < numMembers; i++) {
            const hkClassMember* member = getMember(i);
            if (hkString::strCmp(member->m_name, name) == 0) {
                return i;
            }
        }
        return -1;
    }

    // IDA: ?getDeclaredMemberIndexByName@hkClass@@QEBAHPEBD@Z @ 0x1407908c0
    int getDeclaredMemberIndexByName(const char* name) const {
        int numMembers = getNumDeclaredMembers();
        for (int i = 0; i < numMembers; i++) {
            const hkClassMember* member = getDeclaredMember(i);
            if (hkString::strCmp(member->m_name, name) == 0) {
                return i;
            }
        }
        return -1;
    }

    // IDA: ?getObjectSize@hkClass@@QEBAHXZ @ 0x140790940
    int getObjectSize() const { return m_objectSize; }

    // IDA: ?setObjectSize@hkClass@@QEAAXH@Z @ 0x140790950
    void setObjectSize(int size) { m_objectSize = size; }

    // IDA: ?hasVtable@hkClass@@QEBA?AVhkBool@@XZ @ 0x140790960
    hkBool hasVtable() const {
        // Walk to the root class and check if it has vtable info
        const hkClass* root = this;
        while (root->m_parent != nullptr) {
            root = root->m_parent;
        }
        // Check if root has vtable (simplified - actual implementation would check m_numVtableEntries or similar)
        return hkBool(false);  // Stub - requires full vtable member
    }

    // IDA: ?getDescribedVersion@hkClass@@QEBAHXZ @ 0x1407909a0
    int getDescribedVersion() const { return m_describedVersion; }

    // IDA: ?retrieveMember@hkClass@@AEBA?AUhkResult@@HAEAPEBXAEAPEBVhkClassMember@@@Z @ 0x1407909b0
    hkResult retrieveMember(int memberIndex, const void** defaultOut, const hkClassMember** memberOut) const;

    // IDA: ?hasDefault@hkClass@@QEBAIH@Z @ 0x140790a50
    bool hasDefault(int memberIndex) const {
        hkResult result;
        const void* defaultPtr = nullptr;
        const hkClassMember* member = nullptr;
        return retrieveMember(memberIndex, &defaultPtr, &member).m_enum == HK_SUCCESS;
    }

    // IDA: ?hasDeclaredDefault@hkClass@@QEBAIH@Z @ 0x140790a90
    bool hasDeclaredDefault(int declaredIndex) const {
        if (m_defaults) {
            int val = static_cast<const int*>(m_defaults)[declaredIndex];
            return val >= 0 || val == -2;
        }
        return false;
    }

    // IDA: ?getDefault@hkClass@@QEBAPEBXH@Z @ 0x140790ac0
    const void* getDefault(int memberIndex) const {
        hkResult result;
        const void* defaultPtr = nullptr;
        const hkClassMember* member = nullptr;
        retrieveMember(memberIndex, &defaultPtr, &member);
        if (result.m_enum == HK_SUCCESS) {
            return defaultPtr;
        }
        return nullptr;
    }

    // IDA: ?getAttribute@hkClass@@QEBAPEBUhkVariant@@PEBD@Z @ 0x140790c70
    const hkVariant* getAttribute(const char* id) const;

    // IDA: ?getDeclaredEnumByName@hkClass@@QEBAPEBVhkClassEnum@@PEBD@Z @ 0x140790d10
    const hkClassEnum* getDeclaredEnumByName(const char* name) const {
        int numEnums = getNumDeclaredEnums();
        for (int i = 0; i < numEnums; i++) {
            const hkClassEnum* enumPtr = getDeclaredEnum(i);
            if (hkString::strCmp(enumPtr->m_name, name) == 0) {
                return enumPtr;
            }
        }
        return nullptr;
    }

    // IDA: ?getFlags@hkClass@@QEBAAEBV?$hkFlags@W4FlagValues@hkClass@@I@@XZ @ 0x140790c90
    const hkFlags<FlagValues, unsigned int>& getFlags() const { return m_flags; }

    // IDA: ?getFlags@hkClass@@QEAAAEAV?$hkFlags@W4FlagValues@hkClass@@I@@XZ @ 0x140790ca0
    hkFlags<FlagValues, unsigned int>& getFlags() { return m_flags; }

    // Static class methods (stubs for now)
    static hkClass* staticClass() { return nullptr; }
};

// hkRootLevelContainer - Root level container for Havok resources
class hkRootLevelContainer {
public:
    void* m_data;

    hkRootLevelContainer() : m_data(nullptr) {}

    // IDA: ??$findObject@VhkaiNavMesh@@@hkRootLevelContainer@@QEBAPEAVhkaiNavMesh@@PEBX@Z @ 0x1402fa690
    template<typename T>
    T* findObject(const void* prevObject = nullptr) {
        // Stub - would call findObjectByType with T::staticClass()->getName()
        return nullptr;
    }

    void* findObjectByType(const char* typeName, const void* prevObject) {
        // Stub
        return nullptr;
    }
};

// hkResource - Havok resource container
class hkResource {
public:
    void* m_contents;

    hkResource() : m_contents(nullptr) {}

    // IDA: ??$getContents@VhkRootLevelContainer@@@hkResource@@QEBAPEAVhkRootLevelContainer@@XZ @ 0x1402fa630
    template<typename T>
    T* getContents() {
        // Stub - would call getContentsPointer with T::staticClass()->getName()
        return reinterpret_cast<T*>(m_contents);
    }

    void* getContentsPointer(const char* typeName, const void* registry) {
        // Stub
        return m_contents;
    }
};

// hkTypeInfo - Type information for Havok reflection/serialization
// IDA: finishLoadedObject @ 0x1407b3a40, cleanupLoadedObject @ 0x1407b3aa0
class hkTypeInfo {
public:
    const char* m_typeName;
    int m_typeSize;
    void (*m_finishLoadedObjectFunction)(void*, int);
    void (*m_cleanupLoadedObjectFunction)(void*);

    // IDA: ?finishLoadedObject@hkTypeInfo@@QEBAXPEAXH@Z @ 0x1407b3a40
    void finishLoadedObject(void* ptr, unsigned int finishFlag) const {
        if (m_finishLoadedObjectFunction) {
            m_finishLoadedObjectFunction(ptr, finishFlag);
        }
    }

    // IDA: ?finishLoadedObjectWithoutTracker@hkTypeInfo@@QEBAXPEAXH@Z @ 0x1407b3a70
    void finishLoadedObjectWithoutTracker(void* ptr, unsigned int finishFlag) const {
        if (m_finishLoadedObjectFunction) {
            m_finishLoadedObjectFunction(ptr, finishFlag);
        }
    }

    // IDA: ?cleanupLoadedObject@hkTypeInfo@@QEBAXPEAX@Z @ 0x1407b3aa0
    void cleanupLoadedObject(void* ptr) const {
        if (m_cleanupLoadedObjectFunction) {
            m_cleanupLoadedObjectFunction(ptr);
        }
    }
};

// hkTypeInfoRegistry - Type info registry
class hkTypeInfoRegistry {
public:
    void* m_data;

    hkTypeInfoRegistry() : m_data(nullptr) {}
};

// hkClassNameRegistry - Class name registry
class hkClassNameRegistry {
public:
    void* m_data;

    hkClassNameRegistry() : m_data(nullptr) {}
};

// hkBuiltinTypeRegistry - Built-in type registry
class hkBuiltinTypeRegistry {
public:
    hkTypeInfoRegistry* m_typeInfoRegistry;
    hkClassNameRegistry* m_classNameRegistry;

    hkBuiltinTypeRegistry() : m_typeInfoRegistry(nullptr), m_classNameRegistry(nullptr) {}

    // IDA: ?getLoadedObjectRegistry@hkBuiltinTypeRegistry@@QEAAPEAVhkTypeInfoRegistry@@XZ @ 0x1401047a0
    hkTypeInfoRegistry* getLoadedObjectRegistry() {
        return getTypeInfoRegistry();
    }

    // IDA: getTypeInfoRegistry is called by getLoadedObjectRegistry
    hkTypeInfoRegistry* getTypeInfoRegistry() {
        return m_typeInfoRegistry;
    }

    // IDA: getClassNameRegistry
    hkClassNameRegistry* getClassNameRegistry() {
        return m_classNameRegistry;
    }
};

// Forward declarations for hkBaseSystem dependencies
class hkFileSystem;
class hkDefaultError;
class hkSocket;

// hkSingletonInitNode - Node in singleton initialization linked list
// IDA: Used by hkBaseSystem::initSingletons @ 0x14078e5a0
struct hkSingletonInitNode {
    hkSingletonInitNode* m_next;
    void** m_value;
    void* (*m_createFunc)();
};

// Global singleton initialization list
// IDA: hkSingletonInitList referenced in initSingletons
extern hkSingletonInitNode* hkSingletonInitList;

// hkBaseSystemIsInitialized - Global initialization flag
// IDA: Referenced by isInitialized @ 0x14078e370
extern hkBool hkBaseSystemIsInitialized;

// hkBaseSystem - Havok base system initialization/management
// IDA: Multiple functions at 0x14078exxx
namespace hkBaseSystem {
    // IDA: ?isInitialized@hkBaseSystem@@YA?AVhkBool@@XZ @ 0x14078e370
    inline hkBool isInitialized() {
        return hkBaseSystemIsInitialized;
    }

    // IDA: ?initThread@hkBaseSystem@@YA?AUhkResult@@PEAVhkMemoryRouter@@@Z @ 0x14078e4b0
    hkResult initThread(hkMemoryRouter* memoryRouter);

    // IDA: ?initSingletons@hkBaseSystem@@YAXXZ @ 0x14078e5a0
    void initSingletons();

    // IDA: ?init@hkBaseSystem@@YA?AUhkResult@@PEAVhkMemoryRouter@@P6AXPEBDPEAX@Z2@Z @ 0x14078e730
    hkResult init(hkMemoryRouter* memoryRouter,
                   void (*errorReportFunction)(const char*, void*) = nullptr,
                   void* errorReportObject = nullptr);

    // IDA: ?quitSingletons@hkBaseSystem@@YAXXZ @ 0x14078e870
    void quitSingletons();

    // IDA: ?quit@hkBaseSystem@@YA?AUhkResult@@XZ @ 0x14078e990
    hkResult quit();

    // IDA: ?quitThread@hkBaseSystem@@YA?AUhkResult@@XZ
    hkResult quitThread();
}

// hkFileSystem - Havok file system interface
// IDA: hkWin32FileSystem vtable referenced in hkBaseSystem::init
class hkFileSystem : public hkReferencedObject {
public:
    // IDA: Virtual methods for file operations
    virtual ~hkFileSystem() {}

    // readFile, writeFile, etc.
};

// hkDefaultError - Default error handler
// IDA: Used by hkBaseSystem::init
class hkDefaultError : public hkReferencedObject {
public:
    void (*m_errorReportFunc)(const char*, void*);
    void* m_errorReportObject;

    // IDA: hkDefaultError constructor
    hkDefaultError(void (*errorReportFunc)(const char*, void*) = nullptr, void* errorReportObject = nullptr)
        : m_errorReportFunc(errorReportFunc), m_errorReportObject(errorReportObject) {}
};

// hkError - Error handling singleton type (alias for hkDefaultError)
using hkError = hkDefaultError;

// hkSocket - Socket base class for network operations
// IDA: Referenced in hkBaseSystem::quit
class hkSocket {
public:
    static hkBool s_platformNetInitialized;
    static void (*s_platformNetQuit)();

    // IDA: Socket platform initialization
    static void initPlatformNet() {
        s_platformNetInitialized.m_bool = true;
    }
};

// hkDummySingleton - Dummy singleton for force linkage
// IDA: Referenced in hkBaseSystem::init
class hkDummySingleton : public hkReferencedObject {
public:
    // IDA: forceLinkage method
    void forceLinkage() {}
};

// hkMonitorStream - Monitor stream for profiling/debugging
// IDA: hkMonitorStream::init @ referenced in hkBaseSystem::initThread
namespace hkMonitorStream {
    // IDA: ?init@hkMonitorStream@@SAXXZ
    inline void init() {
        // Stub - initializes monitor stream for profiling
    }
}

// hkInplaceArray - In-place array with fixed storage
// IDA: Used by hkBaseSystem::quitSingletons
template<typename T, int N, typename Allocator = hkContainerHeapAllocator>
class hkInplaceArray : public hkArray<T, Allocator> {
public:
    T m_storage[N];

    hkInplaceArray() {
        this->m_data = m_storage;
        this->m_size = 0;
        this->m_capacityAndFlags = static_cast<int>(0x80000000 | N);  // Mark as inplace
    }
};

// hkSingleton - Singleton holder for Havok types
// IDA: ?getInstance@?$hkSingleton@VhkBuiltinTypeRegistry@@@@SAAEAVhkBuiltinTypeRegistry@@XZ @ 0x1402fa8c0
template<typename T>
class hkSingleton {
public:
    static T* s_instance;

    static T* getInstance() {
        return s_instance;
    }
};

// hkMultiThreadCheck - Thread checking for debugging
struct hkMultiThreadCheck {
    int m_threadId;
    int m_markCount;

    void enableChecks() {
        // Stub - would enable thread checking
    }
};

// hkReferencedObjectLock - Lock management for referenced objects
// IDA: Used by hkReferencedObject::initializeLock, lockInit
// IDA: ??0hkReferencedObjectLock@@QEAA@XZ @ 0x14078bba0
class hkReferencedObjectLock : public hkReferencedObject {
public:
    hkMultiThreadCheck m_multiThreadCheck;
    hkCriticalSection m_criticalSection;
    hkReferencedObject::LockMode m_lockMode;
    int m_lockCount;

    // IDA: ??0hkReferencedObjectLock@@QEAA@XZ @ 0x14078bba0
    hkReferencedObjectLock() : hkReferencedObject() {
        m_memSizeAndFlags = 0x1FFFF;  // Special marker for lock object
        m_multiThreadCheck.m_threadId = -15;
        m_multiThreadCheck.m_markCount = 0x8000;
        // Initialize critical section with spin count 0xFA0 (4000)
        new (&m_criticalSection) hkCriticalSection(0xFA0);
        m_lockMode = hkReferencedObject::LOCK_MODE_AUTO;
        m_multiThreadCheck.enableChecks();
        m_lockCount = 0;
    }

    // IDA: ??_EhkReferencedObjectLock@@UEAAPEAXI@Z @ 0x14078bc10
    virtual ~hkReferencedObjectLock() {
#ifdef _WIN32
        DeleteCriticalSection(&m_criticalSection.m_section);
#endif
    }
};

// hkSerializeUtil::LoadOptions - Serialization load options
class hkSerializeUtil {
public:
    // LoadOptions for serialization
    class LoadOptions {
    public:
        unsigned int m_storage;
        hkClassNameRegistry* m_classNameReg;
        hkTypeInfoRegistry* m_typeInfoReg;

        // IDA: ??0LoadOptions@hkSerializeUtil@@QEAA@W4LoadOptionBits@1@@Z @ 0x1402fbf30
        LoadOptions(unsigned int bits = 0) : m_storage(bits), m_classNameReg(nullptr), m_typeInfoReg(nullptr) {}

        // IDA: ?getClassNameRegistry@LoadOptions@hkSerializeUtil@@QEBAPEBVhkClassNameRegistry@@XZ @ 0x1407b3c70
        const hkClassNameRegistry* getClassNameRegistry() {
            if (m_classNameReg) return m_classNameReg;
            // Fall back to builtin registry
            auto* instance = hkSingleton<hkBuiltinTypeRegistry>::getInstance();
            return instance ? instance->getClassNameRegistry() : nullptr;
        }

        // IDA: ?getTypeInfoRegistry@LoadOptions@hkSerializeUtil@@QEBAPEBVhkTypeInfoRegistry@@XZ @ 0x1407b3ca0
        const hkTypeInfoRegistry* getTypeInfoRegistry() {
            if (m_typeInfoReg) return m_typeInfoReg;
            // Fall back to builtin registry
            auto* instance = hkSingleton<hkBuiltinTypeRegistry>::getInstance();
            return instance ? instance->getTypeInfoRegistry() : nullptr;
        }
    };

    // Static load functions (stubs for now)
    static hkResource* load(void* stream, void* error, LoadOptions* options) {
        return nullptr;
    }
};

// hkStreamReader - Stream reader interface
// IDA: ?skip@hkStreamReader@@UEAAHH@Z @ 0x14079ad20
// Note: In Havok, hkStreamReader inherits from hkReferencedObject
class hkStreamReader : public hkReferencedObject {
public:
    // Virtual read function - must be overridden
    virtual int read(void* buf, int nbytes) = 0;

    // IDA: ?skip@hkStreamReader@@UEAAHH@Z @ 0x14079ad20
    virtual int skip(int nbytes) {
        char buf[512];
        int skipped = 0;
        while (nbytes > 0) {
            int toRead = (nbytes > 512) ? 512 : nbytes;
            int n = read(buf, toRead);
            if (n == 0) break;
            skipped += n;
            nbytes -= n;
        }
        return skipped;
    }

    // IDA: ?peek@hkStreamReader@@UEAAHPEAXH@Z @ 0x14079ada0
    virtual int peek(void* buf, int nbytes) {
        return 0;  // Default implementation returns 0
    }

    // Check if stream is OK
    virtual bool isOk() const { return true; }
};

// hkStreamWriter - Stream writer interface
// Note: In Havok, hkStreamWriter inherits from hkReferencedObject
class hkStreamWriter : public hkReferencedObject {
public:
    // Virtual write function - must be overridden
    virtual int write(const void* buf, int nbytes) = 0;

    // IDA: ?tell@hkStreamWriter@@UEBAHXZ @ 0x14079baa0
    virtual int tell() const { return -1; }

    // IDA: ?seekTellSupported@hkStreamWriter@@UEBA?AVhkBool@@XZ @ 0x14079bab0
    virtual hkBool seekTellSupported() const { return hkBool(false); }

    // IDA: ?seek@hkStreamWriter@@UEAA?AUhkResult@@HW4SeekWhence@1@@Z @ 0x14079bac0
    virtual hkResult seek(int offset, int whence) {
        hkResult res;
        res.m_result = 1; // HK_FAILURE
        return res;
    }

    // Flush any buffered data
    virtual void flush() {}

    // Check if stream is OK
    virtual bool isOk() const { return true; }
};

// Forward declarations
class hkStringPtr;
class hkStringBuf;
class hkVector4;

// hkOstream - Output stream wrapper
// IDA: ??0hkOstream@@QEAA@PEAVhkStreamWriter@@@Z @ 0x1407a7770
class hkOstream : public hkReferencedObject {
public:
    hkRefPtr<hkStreamWriter> m_writer;

    // IDA: ??0hkOstream@@QEAA@PEAVhkStreamWriter@@@Z @ 0x1407a7770
    hkOstream(hkStreamWriter* sw);

    // IDA: ??0hkOstream@@QEAA@PEAXHVhkBool@@@Z @ 0x1407a77e0
    hkOstream(void* mem, int memSize, hkBool isString);

    // IDA: ??0hkOstream@@QEAA@PEAVhkMemoryTrack@@@Z @ 0x1407a7890
    hkOstream(hkMemoryTrack* buf);

    // IDA: ??1hkOstream@@UEAA@XZ @ 0x1407a7950
    ~hkOstream() override;

    // IDA: ?flush@hkOstream@@QEAAXXZ @ 0x1407a7a90
    void flush();

    // IDA: ?write@hkOstream@@QEAAHPEBDH@Z @ 0x1407a7ab0
    int write(const char* buf, long long nbytes);

    // IDA: ?setStreamWriter@hkOstream@@QEAAXPEAVhkStreamWriter@@@Z @ 0x1407a7ad0
    void setStreamWriter(hkStreamWriter* newWriter);

    // IDA: ?isOk@hkOstream@@QEBA?AVhkBool@@XZ @ 0x1407a7c30
    bool isOk() const;

    // printf - formatted output
    // IDA: ?printf@hkOstream@@QEAAXPEBDZZ @ 0x1407a7fd0
    void printf(const char* fmt, ...) {
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        m_writer.m_ptr->write(buf, (int)strlen(buf));
    }

    // IDA: ??6hkOstream@@QEAAAEAV0@AEBVhkStringPtr@@@Z @ 0x1407a79c0
    hkOstream& operator<<(const hkStringPtr& str);

    // IDA: ??6hkOstream@@QEAAAEAV0@AEBVhkStringBuf@@@Z @ 0x1407a7a40
    hkOstream& operator<<(const hkStringBuf& str);

    // IDA: ??6hkOstream@@QEAAAEAV0@H@Z @ 0x1407a7dd0
    hkOstream& operator<<(int i) {
        printf("%i", i);
        return *this;
    }

    // IDA: ??6hkOstream@@QEAAAEAV0@I@Z @ 0x1407a7e20
    hkOstream& operator<<(unsigned int i) {
        printf("%u", i);
        return *this;
    }

    // IDA: ??6hkOstream@@QEAAAEAV0@M@Z @ 0x1407a7ed0
    hkOstream& operator<<(float f) {
        printf("%f", f);
        return *this;
    }

    // IDA: ??6hkOstream@@QEAAAEAV0@_J@Z @ 0x1407a7f30
    hkOstream& operator<<(long long i) {
        printf("%lld", i);
        return *this;
    }

    // IDA: ??6hkOstream@@QEAAAEAV0@AEBVhkVector4@@@Z @ 0x1407a8030
    hkOstream& operator<<(const hkVector4& v) {
        printf("[%g,%g,%g,%g]", v.x, v.y, v.z, v.w);
        return *this;
    }

    // IDA: ??6hkOstream@@QEAAAEAV0@AEBVhkQuaternion@@@Z @ 0x1407a8090
    hkOstream& operator<<(const hkQuaternion& q) {
        printf("[%f,%f,%f,(%f)]", q.m_vec.x, q.m_vec.y, q.m_vec.z, q.m_vec.w);
        return *this;
    }

    // IDA: ??6hkOstream@@QEAAAEAV0@AEBVhkSimdReal@@@Z @ 0x1407a7e70
    hkOstream& operator<<(const hkSimdReal& f) {
#ifdef _WIN32
        float val;
        _mm_store_ss(&val, f.m_real);
        printf("%f", val);
#else
        printf("%f", f.m_value);
#endif
        return *this;
    }

    // IDA: ??6hkOstream@@QEAAAEAV0@AEBVhkMatrix3@@@Z @ 0x1407a80f0
    hkOstream& operator<<(const hkMatrix3& m) {
        printf("|%f,%f,%f|\n", m.m_col0.x, m.m_col1.x, m.m_col2.x);
        printf("|%f,%f,%f|\n", m.m_col0.y, m.m_col1.y, m.m_col2.y);
        printf("|%f,%f,%f|\n", m.m_col0.z, m.m_col1.z, m.m_col2.z);
        return *this;
    }

    // IDA: ??6hkOstream@@QEAAAEAV0@AEBVhkTransform@@@Z @ 0x1407a8170
    hkOstream& operator<<(const hkTransform& t);
};

// hkOArchive - Output archive for serialization
// IDA: ??0hkOArchive@@QEAA@PEAVhkStreamWriter@@VhkBool@@@Z @ 0x14079c6d0
class hkOArchive : public hkReferencedObject {
public:
    hkRefPtr<hkStreamWriter> m_writer;
    hkBool m_byteSwap;

    // IDA: ??0hkOArchive@@QEAA@PEAVhkStreamWriter@@VhkBool@@@Z @ 0x14079c6d0
    hkOArchive(hkStreamWriter* sw, hkBool bs = hkBool(false)) {
        m_memSizeAndFlags = 0xFFFFFFFF;
        m_referenceCount = 1;
        if (sw) {
            sw->addReference();
        }
        m_writer.m_ptr = sw;
        m_byteSwap = bs;
    }

    // IDA: ??1hkOArchive@@UEAA@XZ @ 0x14079c7e0
    ~hkOArchive() override {
        hkStreamWriter* writer = m_writer.m_ptr;
        if (writer) {
            writer->removeReference();
        }
        m_writer.m_ptr = nullptr;
    }

    // IDA: ?writeRaw@hkOArchive@@QEAAHPEBXH@Z @ 0x14079ca80
    int writeRaw(const void* buf, long long nbytes) {
        return m_writer.m_ptr->write(buf, (int)nbytes);
    }

    // IDA: ?writeArrayGeneric@hkOArchive@@QEAAXPEBXHH@Z @ 0x14079c880
    void writeArrayGeneric(const char* ptr, int elemSize, int arraySize) {
        if (m_byteSwap.m_bool) {
            char buf[512];
            int totalBytes = arraySize * elemSize;
            int chunkSize = 512;
            int elemsPerChunk = 512 / elemSize;

            for (int offset = 0; offset < totalBytes; offset += chunkSize) {
                if (totalBytes - offset < 512) {
                    chunkSize = totalBytes - offset;
                    elemsPerChunk = chunkSize / elemSize;
                }

                hkString::memCpy(buf, ptr + offset, chunkSize);

                // Byte swap
                if (elemSize == 2) {
                    for (int i = 0; i < elemsPerChunk; i++) {
                        char* p = buf + i * 2;
                        char tmp = p[0];
                        p[0] = p[1];
                        p[1] = tmp;
                    }
                } else if (elemSize == 4) {
                    for (int i = 0; i < elemsPerChunk; i++) {
                        char* p = buf + i * 4;
                        char tmp0 = p[0], tmp1 = p[1], tmp2 = p[2], tmp3 = p[3];
                        p[0] = tmp3; p[1] = tmp2; p[2] = tmp1; p[3] = tmp0;
                    }
                } else if (elemSize == 8) {
                    for (int i = 0; i < elemsPerChunk; i++) {
                        char* p = buf + i * 8;
                        char tmp[8];
                        for (int j = 0; j < 8; j++) tmp[j] = p[j];
                        for (int j = 0; j < 8; j++) p[j] = tmp[7 - j];
                    }
                }

                m_writer.m_ptr->write(buf, chunkSize);
            }
        } else {
            m_writer.m_ptr->write(ptr, arraySize * elemSize);
        }
    }

    // IDA: ?setStreamWriter@hkOArchive@@QEAAXPEAVhkStreamWriter@@@Z @ 0x14079cad0
    void setStreamWriter(hkStreamWriter* newWriter) {
        if (newWriter) {
            newWriter->addReference();
        }
        hkStreamWriter* oldWriter = m_writer.m_ptr;
        if (oldWriter) {
            oldWriter->removeReference();
        }
        m_writer.m_ptr = newWriter;
    }

    // IDA: ?getStreamWriter@hkOArchive@@QEAAPEAVhkStreamWriter@@XZ @ 0x14079cf60
    hkStreamWriter* getStreamWriter() {
        return m_writer.m_ptr;
    }

    // IDA: ?isOk@hkOArchive@@QEBA?AVhkBool@@XZ @ 0x14079caa0
    hkBool isOk() const {
        if (m_writer.m_ptr) {
            return hkBool(m_writer.m_ptr->isOk());
        }
        return hkBool(false);
    }

    // IDA: ?setByteSwap@hkOArchive@@QEAAXVhkBool@@@Z @ 0x14079c6b0
    void setByteSwap(hkBool bs) {
        m_byteSwap = bs;
    }

    // IDA: ?getByteSwap@hkOArchive@@QEBA?AVhkBool@@XZ @ 0x14079c6c0
    hkBool getByteSwap() const {
        return m_byteSwap;
    }

    // IDA: ?write8@hkOArchive@@QEAAXC@Z @ 0x14079c820
    void write8(char val) {
        m_writer.m_ptr->write(&val, 1);
    }

    // IDA: ?write8u@hkOArchive@@QEAAXE@Z @ 0x14079c850
    void write8u(unsigned char val) {
        m_writer.m_ptr->write(&val, 1);
    }

    // IDA: ?write16@hkOArchive@@QEAAXF@Z @ 0x14079cc30
    void write16(short i) {
        writeArrayGeneric(reinterpret_cast<const char*>(&i), 2, 1);
    }

    // IDA: ?write16u@hkOArchive@@QEAAXG@Z @ 0x14079cc60
    void write16u(unsigned short i) {
        writeArrayGeneric(reinterpret_cast<const char*>(&i), 2, 1);
    }

    // IDA: ?write32@hkOArchive@@QEAAXH@Z @ 0x14079cc90
    void write32(int i) {
        writeArrayGeneric(reinterpret_cast<const char*>(&i), 4, 1);
    }

    // IDA: ?write32u@hkOArchive@@QEAAXI@Z @ 0x14079ccc0
    void write32u(unsigned int i) {
        writeArrayGeneric(reinterpret_cast<const char*>(&i), 4, 1);
    }

    // IDA: ?write64@hkOArchive@@QEAAX_J@Z @ 0x14079ccf0
    void write64(long long i) {
        writeArrayGeneric(reinterpret_cast<const char*>(&i), 8, 1);
    }

    // IDA: ?write64u@hkOArchive@@QEAAX_K@Z @ 0x14079cd20
    void write64u(unsigned long long i) {
        writeArrayGeneric(reinterpret_cast<const char*>(&i), 8, 1);
    }

    // IDA: ?writeFloat32@hkOArchive@@QEAAXM@Z @ 0x14079cd50
    void writeFloat32(float f) {
        writeArrayGeneric(reinterpret_cast<const char*>(&f), 4, 1);
    }

    // IDA: ?writeDouble64@hkOArchive@@QEAAXN@Z @ 0x14079cd80
    void writeDouble64(double d) {
        writeArrayGeneric(reinterpret_cast<const char*>(&d), 8, 1);
    }

    // IDA: ?writeArray8@hkOArchive@@QEAAXPEBCH@Z @ 0x14079cdb0
    void writeArray8(const char* array, int nelem) {
        writeArrayGeneric(array, 1, nelem);
    }

    // IDA: ?writeArray8u@hkOArchive@@QEAAXPEBEH@Z @ 0x14079cdd0
    void writeArray8u(const unsigned char* array, int nelem) {
        writeArrayGeneric(reinterpret_cast<const char*>(array), 1, nelem);
    }

    // IDA: ?writeArray16@hkOArchive@@QEAAXPEBFH@Z @ 0x14079cdf0
    void writeArray16(const short* array, int nelem) {
        writeArrayGeneric(reinterpret_cast<const char*>(array), 2, nelem);
    }

    // IDA: ?writeArray16u@hkOArchive@@QEAAXPEBGH@Z @ 0x14079ce10
    void writeArray16u(const unsigned short* array, int nelem) {
        writeArrayGeneric(reinterpret_cast<const char*>(array), 2, nelem);
    }

    // IDA: ?writeArray32@hkOArchive@@QEAAXPEBHH@Z @ 0x14079ce30
    void writeArray32(const int* array, int nelem) {
        writeArrayGeneric(reinterpret_cast<const char*>(array), 4, nelem);
    }

    // IDA: ?writeArray32u@hkOArchive@@QEAAXPEBIH@Z @ 0x14079ce50
    void writeArray32u(const unsigned int* array, int nelem) {
        writeArrayGeneric(reinterpret_cast<const char*>(array), 4, nelem);
    }

    // IDA: ?writeArray64@hkOArchive@@QEAAXPEB_JH@Z @ 0x14079ce70
    void writeArray64(const long long* array, int nelem) {
        writeArrayGeneric(reinterpret_cast<const char*>(array), 8, nelem);
    }

    // IDA: ?writeArray64u@hkOArchive@@QEAAXPEB_KH@Z @ 0x14079ce90
    void writeArray64u(const unsigned long long* array, int nelem) {
        writeArrayGeneric(reinterpret_cast<const char*>(array), 8, nelem);
    }

    // IDA: ?writeArrayFloat32@hkOArchive@@QEAAXPEBMH@Z @ 0x14079ceb0
    void writeArrayFloat32(const float* array, int nelem) {
        writeArrayGeneric(reinterpret_cast<const char*>(array), 4, nelem);
    }

    // IDA: ?writeArrayFloat32@hkOArchive@@QEAAXPEBNH@Z @ 0x14079ced0
    // Note: This appears to be a duplicate signature in IDA - likely for double
    void writeArrayFloat64(const double* array, int nelem) {
        writeArrayGeneric(reinterpret_cast<const char*>(array), 8, nelem);
    }

    // IDA: ?writeArrayDouble64@hkOArchive@@QEAAXPEBNH@Z @ 0x14079cf40
    void writeArrayDouble64(const double* array, int nelem) {
        writeArrayGeneric(reinterpret_cast<const char*>(array), 8, nelem);
    }
};

// hkIstream - Input stream wrapper
// IDA: ??0hkIstream@@QEAA@PEAVhkStreamReader@@@Z @ 0x1409d9b60
// Note: hkStreamReader is referenced, but we use simple pointer here
class hkIstream {
public:
    hkStreamReader* m_streamReader;

    // IDA: ??0hkIstream@@QEAA@PEAVhkStreamReader@@@Z @ 0x1409d9b60
    hkIstream(hkStreamReader* reader) : m_streamReader(reader) {}

    // IDA: ??1hkIstream@@UEAA@XZ @ 0x1409d9bd0
    ~hkIstream() {}

    // IDA: ?read@hkIstream@@QEAAHPEAXH@Z @ 0x1409d9c40
    int read(void* buf, int nbytes) {
        return m_streamReader->read(buf, nbytes);
    }

    // IDA: ??5hkIstream@@QEAAAEAV0@AEAD@Z @ 0x1409d9c10
    hkIstream& operator>>(char& c) {
        m_streamReader->read(&c, 1);
        return *this;
    }

    // IDA: ??5hkIstream@@QEAAAEAV0@AEAF@Z @ 0x1409d9fc0
    hkIstream& operator>>(short& val) {
        m_streamReader->read(&val, sizeof(short));
        return *this;
    }

    // IDA: ??5hkIstream@@QEAAAEAV0@AEAG@Z @ 0x1409da000
    hkIstream& operator>>(unsigned short& val) {
        m_streamReader->read(&val, sizeof(unsigned short));
        return *this;
    }

    // IDA: ??5hkIstream@@QEAAAEAV0@AEAH@Z @ 0x1409da040
    hkIstream& operator>>(int& val) {
        m_streamReader->read(&val, sizeof(int));
        return *this;
    }

    // IDA: ??5hkIstream@@QEAAAEAV0@AEAI@Z @ 0x1409da080
    hkIstream& operator>>(unsigned int& val) {
        m_streamReader->read(&val, sizeof(unsigned int));
        return *this;
    }

    // IDA: ??5hkIstream@@QEAAAEAV0@AEAM@Z @ 0x1409da0c0
    hkIstream& operator>>(float& val) {
        m_streamReader->read(&val, sizeof(float));
        return *this;
    }

    // IDA: ??5hkIstream@@QEAAAEAV0@AEAN@Z @ 0x1409da100
    hkIstream& operator>>(double& val) {
        m_streamReader->read(&val, sizeof(double));
        return *this;
    }

    // IDA: ??5hkIstream@@QEAAAEAV0@AEA_J@Z @ 0x1409da140
    hkIstream& operator>>(long long& val) {
        m_streamReader->read(&val, sizeof(long long));
        return *this;
    }

    // IDA: ??5hkIstream@@QEAAAEAV0@AEA_K@Z @ 0x1409da180
    hkIstream& operator>>(unsigned long long& val) {
        m_streamReader->read(&val, sizeof(unsigned long long));
        return *this;
    }

    // IDA: ?isOk@hkIstream@@QEBA?AVhkBool@@XZ @ 0x1409d9e60
    hkBool isOk() const {
        return hkBool(m_streamReader && m_streamReader->isOk());
    }

    // IDA: ?setStreamReader@hkIstream@@QEAAXPEAVhkStreamReader@@@Z @ 0x1409d9c60
    void setStreamReader(hkStreamReader* reader) {
        m_streamReader = reader;
    }
};

// hkIArchive - Input archive for serialization
// IDA: ??0hkIArchive@@QEAA@PEAVhkStreamReader@@VhkBool@@@Z @ 0x1409db530
class hkIArchive : public hkReferencedObject {
public:
    hkRefPtr<hkStreamReader> m_streamReader;
    hkBool m_byteSwap;

    // IDA: ??0hkIArchive@@QEAA@PEAVhkStreamReader@@VhkBool@@@Z @ 0x1409db530
    hkIArchive(hkStreamReader* sb, hkBool bs = hkBool(false)) {
        m_memSizeAndFlags = 0xFFFFFFFF;
        m_referenceCount = 1;
        if (sb) {
            sb->addReference();
        }
        m_streamReader.m_ptr = sb;
        m_byteSwap = bs;
    }

    // IDA: ??0hkIArchive@@QEAA@PEBXHVhkBool@@@Z @ 0x1409db590
    hkIArchive(const void* mem, int memSize, hkBool byteswap = hkBool(false)) {
        m_memSizeAndFlags = 0xFFFFFFFF;
        m_referenceCount = 1;
        m_streamReader.m_ptr = nullptr;
        m_byteSwap = byteswap;
        // Create memory stream reader
        // Note: hkMemoryStreamReader needs to be defined, using placeholder
        // hkMemoryStreamReader* reader = new hkMemoryStreamReader(mem, memSize, MEMORY_INPLACE);
        // m_streamReader.m_ptr = reader;
    }

    // IDA: ??0hkIArchive@@QEAA@PEBDVhkBool@@@Z @ 0x1409db800
    hkIArchive(const char* filename, hkBool bs = hkBool(false)) {
        // Special marker for file-based archive
        m_memSizeAndFlags = 0x1FFFF;
        m_referenceCount = 1;
        m_streamReader.m_ptr = nullptr;
        m_byteSwap = bs;
        // Note: Would open file using hkFileSystem
        // hkRefNew<hkStreamReader> reader = hkSingleton<hkFileSystem>::s_instance->openReader(filename, 1);
        // m_streamReader.m_ptr = reader.m_pntr;
    }

    // IDA: ??1hkIArchive@@UEAA@XZ @ 0x1409db640
    ~hkIArchive() override {
        hkStreamReader* reader = m_streamReader.m_ptr;
        if (reader) {
            reader->removeReference();
        }
        m_streamReader.m_ptr = nullptr;
    }

    // IDA: ?readRaw@hkIArchive@@QEAAHPEAXH@Z @ 0x1409db7a0
    int readRaw(void* buf, long long nbytes) {
        return m_streamReader.m_ptr->read(buf, (int)nbytes);
    }

    // IDA: ?readArrayGeneric@hkIArchive@@QEAAXPEAXHH@Z @ 0x1409db680
    void readArrayGeneric(char* array, int elemsize, unsigned int arraySize) {
        m_streamReader.m_ptr->read(array, arraySize * elemsize);
        if (m_byteSwap.m_bool) {
            if (elemsize == 2) {
                for (unsigned int i = 0; i < arraySize; i++) {
                    char* p = array + i * 2;
                    char tmp = p[0];
                    p[0] = p[1];
                    p[1] = tmp;
                }
            } else if (elemsize == 4) {
                for (unsigned int i = 0; i < arraySize; i++) {
                    char* p = array + i * 4;
                    char tmp0 = p[0], tmp1 = p[1], tmp2 = p[2], tmp3 = p[3];
                    p[0] = tmp3; p[1] = tmp2; p[2] = tmp1; p[3] = tmp0;
                }
            } else if (elemsize == 8) {
                for (unsigned int i = 0; i < arraySize; i++) {
                    char* p = array + i * 8;
                    char tmp[8];
                    for (int j = 0; j < 8; j++) tmp[j] = p[j];
                    for (int j = 0; j < 8; j++) p[j] = tmp[7 - j];
                }
            }
        }
    }

    // IDA: ?readArrayFloat32@hkIArchive@@QEAAXPEANH@Z @ 0x1409db870
    void readArrayFloat32(double* buf, int nelem) {
        if (nelem > 0) {
            for (int i = 0; i < nelem; i++) {
                float val;
                readArrayGeneric((char*)&val, 4, 1);
                buf[i] = val;
            }
        }
    }

    // IDA: ?setStreamReader@hkIArchive@@QEAAXPEAVhkStreamReader@@@Z @ 0x1409db7c0
    void setStreamReader(hkStreamReader* newBuf) {
        if (newBuf) {
            newBuf->addReference();
        }
        hkStreamReader* oldReader = m_streamReader.m_ptr;
        if (oldReader) {
            oldReader->removeReference();
        }
        m_streamReader.m_ptr = newBuf;
    }

    // IDA: ?getStreamReader@hkIArchive@@QEAAPEAVhkStreamReader@@XZ @ 0x1409db930
    hkStreamReader* getStreamReader() {
        return m_streamReader.m_ptr;
    }

    // IDA: ?isOk@hkIArchive@@QEBA?AVhkBool@@XZ @ 0x1409db8f0
    hkBool isOk() const {
        hkStreamReader* reader = m_streamReader.m_ptr;
        if (reader && reader->isOk()) {
            return hkBool(true);
        }
        return hkBool(false);
    }
};

// hkSeekableStreamReader - Seekable stream reader base class
class hkSeekableStreamReader : public hkStreamReader {
public:
    enum SeekWhence {
        SEEK_WHENCE_SET = 0,  // Seek from beginning
        SEEK_WHENCE_CUR = 1,  // Seek from current position
        SEEK_WHENCE_END = 2   // Seek from end
    };

    // Virtual seek function
    virtual hkResult seek(int relOffset, SeekWhence whence) = 0;

    // Virtual tell function
    virtual int tell() const = 0;
};

// hkMemoryStreamReader - Memory-based stream reader
// IDA: ??0hkMemoryStreamReader@@QEAA@PEBXHW4MemoryType@0@@Z @ 0x1409e5470
class hkMemoryStreamReader : public hkSeekableStreamReader {
public:
    enum MemoryType {
        MEMORY_COPY = 0,    // Copy the memory (caller owns original)
        MEMORY_INPLACE = 1, // Use memory in place (caller owns memory)
        MEMORY_TAKE = 2     // Take ownership of memory (reader will free)
    };

    char* m_buf;
    unsigned int m_bufSize;
    unsigned int m_bufCurrent;
    MemoryType m_memType;
    hkBool m_hitEof;

    // IDA: ??0hkMemoryStreamReader@@QEAA@PEBXHW4MemoryType@0@@Z @ 0x1409e5470
    hkMemoryStreamReader(const void* mem, unsigned int memSize, MemoryType mt) {
        m_memType = mt;
        m_bufCurrent = 0;
        m_memSizeAndFlags = 0xFFFFFFFF;
        m_bufSize = memSize;
        m_referenceCount = 1;
        m_hitEof = hkBool(false);

        if (m_memType >= MEMORY_INPLACE) {
            m_buf = const_cast<char*>(static_cast<const char*>(mem));
        } else {
            // MEMORY_COPY: allocate and copy
            int allocSize = memSize;
            void* newBuf = hkContainerTempAllocator::s_alloc.bufAlloc(allocSize);
            m_buf = static_cast<char*>(newBuf);
            hkString::memCpy(m_buf, mem, memSize);
        }
    }

    // IDA: ??1hkMemoryStreamReader@@UEAA@XZ @ 0x1409e5520
    ~hkMemoryStreamReader() override {
        if (m_memType < MEMORY_INPLACE) {
            // MEMORY_COPY: free the copied buffer
            hkContainerTempAllocator::s_alloc.bufFree(m_buf, m_bufSize);
        }
    }

    // IDA: ?read@hkMemoryStreamReader@@UEAAHPEAXH@Z @ 0x1409e5300
    int read(void* buf, int nbytes) override {
        unsigned int avail = m_bufSize - m_bufCurrent;
        unsigned int toRead = (nbytes > (int)avail) ? avail : nbytes;
        hkString::memCpy(buf, m_buf + m_bufCurrent, toRead);
        m_bufCurrent += toRead;
        if (toRead == 0 && nbytes > 0) {
            m_hitEof = hkBool(true);
        }
        return toRead;
    }

    // IDA: ?skip@hkMemoryStreamReader@@UEAAHH@Z @ 0x1409e5370
    int skip(int nbytes) override {
        unsigned int avail = m_bufSize - m_bufCurrent;
        unsigned int toSkip = (nbytes > (int)avail) ? avail : nbytes;
        m_bufCurrent += toSkip;
        if (toSkip == 0 && nbytes > 0) {
            m_hitEof = hkBool(true);
        }
        return toSkip;
    }

    // IDA: ?peek@hkMemoryStreamReader@@UEAAHPEAXH@Z @ 0x1409e53c0
    int peek(void* buf, int nbytes) override {
        unsigned int avail = m_bufSize - m_bufCurrent;
        unsigned int toPeek = (nbytes > (int)avail) ? avail : nbytes;
        hkString::memCpy(buf, m_buf + m_bufCurrent, toPeek);
        return toPeek;
    }

    // IDA: ?isOk@hkMemoryStreamReader@@UEBA?AVhkBool@@XZ @ 0x1409e53a0
    bool isOk() const override {
        return !m_hitEof.m_bool;
    }

    // IDA: ?tell@hkMemoryStreamReader@@UEBAHXZ @ 0x1409e52e0
    int tell() const override {
        return m_bufCurrent;
    }

    // IDA: ?seek@hkMemoryStreamReader@@UEAA?AUhkResult@@HW4SeekWhence@hkSeekableStreamReader@@@Z @ 0x1409e5400
    hkResult seek(int relOffset, SeekWhence whence) override {
        int newPos = 0;
        if (whence == SEEK_WHENCE_SET) {
            newPos = relOffset;
        } else if (whence == SEEK_WHENCE_CUR) {
            newPos = m_bufCurrent + relOffset;
        } else { // SEEK_WHENCE_END
            newPos = m_bufSize - relOffset;
        }

        hkResult res;
        if (newPos < 0) {
            m_bufCurrent = 0;
            m_hitEof = hkBool(false);
            res.m_result = 1; // HK_FAILURE
        } else if (newPos > (int)m_bufSize) {
            m_bufCurrent = m_bufSize;
            m_hitEof = hkBool(false);
            res.m_result = 1; // HK_FAILURE
        } else {
            m_bufCurrent = newPos;
            m_hitEof = hkBool(false);
            res.m_result = 0; // HK_SUCCESS
        }
        return res;
    }
};

// hkBufferedStreamReader - Buffered stream reader
// IDA: ??0hkBufferedStreamReader@@QEAA@PEAVhkStreamReader@@H@Z @ 0x1407a8b90
class hkBufferedStreamReader : public hkSeekableStreamReader {
public:
    // Buffer structure
    struct Buffer {
        char* begin;
        int current;
        int size;
        int capacity;

        // IDA: ??0Buffer@hkBufferedStreamReader@@QEAA@H@Z @ 0x1407a8b10
        Buffer(int cap) {
            begin = static_cast<char*>(hkContainerTempAllocator::s_alloc.bufAlloc(cap));
            capacity = cap;
            current = 0;
            size = 0;
        }

        // IDA: ??1Buffer@hkBufferedStreamReader@@QEAA@XZ @ 0x1407a8b60
        ~Buffer() {
            if (begin) {
                hkContainerTempAllocator::s_alloc.bufFree(begin, capacity);
            }
        }
    };

    hkStreamReader* m_stream;
    hkSeekableStreamReader* m_seekStream;
    Buffer m_buf;

    // IDA: ??0hkBufferedStreamReader@@QEAA@PEAVhkStreamReader@@H@Z @ 0x1407a8b90
    hkBufferedStreamReader(hkStreamReader* s, int bufSize)
        : m_buf(bufSize) {
        m_stream = s;
        m_memSizeAndFlags = 0xFFFFFFFF;
        m_referenceCount = 1;
        m_seekStream = nullptr; // s->isSeekTellSupported() would be called
        if (m_stream) {
            m_stream->addReference();
        }
    }

    // IDA: ??1hkBufferedStreamReader@@UEAA@XZ @ 0x1407a8c00
    ~hkBufferedStreamReader() override {
        if (m_stream) {
            m_stream->removeReference();
        }
    }

    // IDA: ?isOk@hkBufferedStreamReader@@UEBA?AVhkBool@@XZ @ 0x1407a8760
    bool isOk() const override {
        if (m_buf.current != m_buf.size) {
            return true;
        }
        return m_stream && m_stream->isOk();
    }

    // IDA: ?isSeekTellSupported@hkBufferedStreamReader@@UEAAPEAVhkSeekableStreamReader@@XZ @ 0x1407a87b0
    hkSeekableStreamReader* isSeekTellSupported() {
        return m_seekStream ? this : nullptr;
    }

    // IDA: ?seek@hkBufferedStreamReader@@UEAA?AUhkResult@@HW4SeekWhence@hkSeekableStreamReader@@@Z @ 0x1407a87d0
    hkResult seek(int offset, SeekWhence whence) override {
        m_buf.current = 0;
        m_buf.size = 0;
        if (m_seekStream) {
            return m_seekStream->seek(offset, whence);
        }
        hkResult res;
        res.m_result = 1; // HK_FAILURE
        return res;
    }

    // IDA: ?tell@hkBufferedStreamReader@@UEBAHXZ @ 0x1407a8800
    int tell() const override {
        if (!m_seekStream) {
            return -1;
        }
        int base = m_seekStream->tell();
        if (base < 0) {
            return -1;
        }
        return base + m_buf.current - m_buf.size;
    }

    // IDA: ?read@hkBufferedStreamReader@@UEAAHPEAXH@Z @ 0x1407a88e0
    int read(void* buf, int nbytes) override {
        char* outBuf = static_cast<char*>(buf);
        int totalRead = 0;
        int remaining = nbytes;

        // First, use buffered data
        int avail = m_buf.size - m_buf.current;
        if (remaining <= avail) {
            hkString::memCpy(outBuf, m_buf.begin + m_buf.current, remaining);
            m_buf.current += remaining;
            return remaining;
        }

        // Copy available buffered data
        if (avail > 0) {
            hkString::memCpy(outBuf, m_buf.begin + m_buf.current, avail);
            remaining -= avail;
            totalRead += avail;
            outBuf += avail;
        }

        // Refill buffer and continue reading
        m_buf.current = 0;
        while (remaining > 0) {
            int n = m_stream->read(m_buf.begin, m_buf.capacity);
            if (n == 0) break;
            m_buf.size = n;

            int toCopy = (remaining < n) ? remaining : n;
            hkString::memCpy(outBuf, m_buf.begin, toCopy);
            m_buf.current = toCopy;
            remaining -= toCopy;
            totalRead += toCopy;
            outBuf += toCopy;

            if (toCopy < n) break; // Some data left in buffer
        }

        return totalRead;
    }

    // IDA: ?skip@hkBufferedStreamReader@@UEAAHH@Z @ 0x1407a8990
    int skip(int nbytes) override {
        int avail = m_buf.size - m_buf.current;
        if (nbytes <= avail) {
            m_buf.current += nbytes;
            return nbytes;
        }

        // Skip buffered data
        int skipped = avail;
        nbytes -= avail;
        m_buf.current = 0;
        m_buf.size = 0;

        // Skip in underlying stream
        int streamSkipped = m_stream->skip(nbytes);
        skipped += streamSkipped;

        return skipped;
    }

    // IDA: ?peek@hkBufferedStreamReader@@UEAAHPEAXH@Z @ 0x1407a8a20
    int peek(void* buf, int nbytes) override {
        if (nbytes > m_buf.capacity - 512) {
            return -1;
        }

        int avail = m_buf.size - m_buf.current;
        if (nbytes > avail) {
            // Need to refill buffer
            // Move remaining data to start
            int remaining = avail;
            int newCurrent = 512 - (remaining % 512);
            if (newCurrent != m_buf.current && remaining > 0) {
                hkString::memMove(m_buf.begin + newCurrent, m_buf.begin + m_buf.current, remaining);
            }
            m_buf.current = newCurrent;

            // Read more data
            int space = m_buf.capacity - (newCurrent + remaining);
            int n = m_stream->read(m_buf.begin + newCurrent + remaining, space);
            m_buf.size = newCurrent + remaining + n;
        }

        int toPeek = m_buf.size - m_buf.current;
        if (nbytes < toPeek) toPeek = nbytes;
        hkString::memCpy(buf, m_buf.begin + m_buf.current, toPeek);
        return toPeek;
    }
};

// hkArrayStreamWriter - Array-based stream writer
// IDA: ??0hkArrayStreamWriter@@QEAA@PEAV?$hkArray@DUhkContainerHeapAllocator@@@@W4ArrayOwnership@0@@Z @ 0x14079cf70
class hkArrayStreamWriter : public hkStreamWriter {
public:
    enum ArrayOwnership {
        ARRAY_BORROW = 0,  // Caller owns the array
        ARRAY_TAKE = 1     // Stream writer owns the array
    };

    hkArrayBase<char>* m_arr;
    hkMemoryAllocator* m_allocator;
    int m_offset;
    ArrayOwnership m_ownerShip;

    // IDA: ??0hkArrayStreamWriter@@QEAA@PEAV?$hkArray@DUhkContainerHeapAllocator@@@@W4ArrayOwnership@0@@Z @ 0x14079cf70
    hkArrayStreamWriter(hkArrayBase<char>* arr, ArrayOwnership o) {
        m_memSizeAndFlags = 0xFFFFFFFF;
        m_arr = arr;
        m_allocator = &hkContainerHeapAllocator::s_alloc;
        m_referenceCount = 1;
        m_offset = arr->m_size;
        m_ownerShip = o;

        // Ensure null terminator
        int needed = arr->m_size + 1;
        int capacity = arr->m_capacityAndFlags & 0x3FFFFFFF;
        if (capacity < needed) {
            int newCap = capacity * 2;
            if (needed > newCap) newCap = needed;
            // Reserve more space
            hkArrayUtil::_reserve(*m_allocator, reinterpret_cast<void**>(&m_arr), newCap, 1);
        }
        m_arr->m_data[m_arr->m_size] = 0;
    }

    // IDA: ??1hkArrayStreamWriter@@UEAA@XZ @ 0x14079d020
    ~hkArrayStreamWriter() override {
        if (m_ownerShip == ARRAY_TAKE && m_arr) {
            // Free the array
            hkContainerHeapAllocator::s_alloc.bufFree(m_arr->m_data, m_arr->m_capacityAndFlags & 0x3FFFFFFF);
        }
    }

    // IDA: ?write@hkArrayStreamWriter@@UEAAHPEBXH@Z @ 0x1407b1f30
    int write(const void* mem, int size) override {
        int avail = m_arr->m_size - m_offset;
        if (size > avail) {
            // Need to expand array
            int newSize = size + m_arr->m_size - avail;
            int capacity = m_arr->m_capacityAndFlags & 0x3FFFFFFF;
            int needed = newSize + 1;
            if (capacity < needed) {
                int newCap = capacity * 2;
                if (needed > newCap) newCap = needed;
                hkArrayUtil::_reserve(*m_allocator, reinterpret_cast<void**>(&m_arr), newCap, 1);
            }
            m_arr->m_size = newSize;
            m_arr->m_data[newSize] = 0;
        }

        hkString::memCpy(m_arr->m_data + m_offset, mem, size);
        m_offset += size;
        return size;
    }

    // IDA: ?clear@hkArrayStreamWriter@@UEAAXXZ @ 0x1407b1ec0
    void clear() {
        m_arr->m_size = m_offset;
        m_arr->m_data[m_offset] = 0;
    }

    // IDA: ?seek@hkArrayStreamWriter@@UEAA?AUhkResult@@HW4SeekWhence@hkStreamWriter@@@Z @ 0x1407b2000
    hkResult seek(int offset, int whence) override {
        hkResult result;
        int newOffset = m_offset;

        if (whence == 0) {  // SEEK_SET
            newOffset = offset;
        } else if (whence == 1) {  // SEEK_CUR
            newOffset += offset;
        } else if (whence == 2) {  // SEEK_END
            newOffset = m_arr->m_size - offset;
        }

        if (newOffset < 0) {
            result.m_result = 1;  // HK_FAILURE
            return result;
        }

        if (newOffset > m_arr->m_size) {
            // Need to expand array
            int needed = newOffset + 1;
            int capacity = m_arr->m_capacityAndFlags & 0x3FFFFFFF;
            if (capacity < needed) {
                int newCap = capacity * 2;
                if (needed > newCap) newCap = needed;
                hkArrayUtil::_reserve(*m_allocator, reinterpret_cast<void**>(&m_arr), newCap, 1);
            }
            // Zero-fill new space
            int gap = newOffset - m_arr->m_size;
            if (gap > 0) {
                memset(&m_arr->m_data[m_arr->m_size], 0, gap);
            }
            m_arr->m_size = newOffset + 1;
            m_arr->m_data[newOffset] = 0;
        }

        m_offset = newOffset;
        result.m_result = HK_SUCCESS;
        return result;
    }
};

// hkBufferedStreamWriter - Buffered stream writer
// IDA: ??0hkBufferedStreamWriter@@QEAA@PEAVhkStreamWriter@@H@Z @ 0x1407a85f0
class hkBufferedStreamWriter : public hkStreamWriter {
public:
    char* m_buf;
    int m_bufSize;
    int m_bufCapacity;
    hkBool m_ownBuffer;
    hkStreamWriter* m_stream;

    // IDA: ??0hkBufferedStreamWriter@@QEAA@PEAVhkStreamWriter@@H@Z @ 0x1407a85f0
    hkBufferedStreamWriter(hkStreamWriter* s, int bufSize) {
        m_stream = s;
        m_memSizeAndFlags = 0xFFFFFFFF;
        m_referenceCount = 1;
        m_ownBuffer.m_bool = true;
        if (m_stream) {
            m_stream->addReference();
        }
        hkMemoryRouter* router = hkMemoryRouter::getInstance();
        m_buf = static_cast<char*>(hkMemoryRouter::alignedAlloc(*router->m_heap, bufSize, 64));
        m_bufSize = 0;
        m_bufCapacity = bufSize;
    }

    // IDA: ??0hkBufferedStreamWriter@@QEAA@PEAXHVhkBool@@@Z @ 0x1407a8580
    hkBufferedStreamWriter(char* mem, unsigned int memSize, hkBool memoryIsString) {
        m_memSizeAndFlags = 0xFFFFFFFF;
        m_buf = mem;
        m_referenceCount = 1;
        m_stream = nullptr;
        m_bufSize = 0;
        int capacity = memoryIsString.m_bool ? (memSize - 1) : memSize;
        m_bufCapacity = capacity;
        m_ownBuffer.m_bool = false;
        if (memoryIsString.m_bool) {
            hkString::memSet(mem, 0, memSize);
        }
    }

    // IDA: ??1hkBufferedStreamWriter@@UEAA@XZ @ 0x1407a8680
    ~hkBufferedStreamWriter() override {
        flush();
        if (m_stream) {
            m_stream->removeReference();
        }
        if (m_ownBuffer.m_bool && m_buf) {
            hkMemoryRouter* router = hkMemoryRouter::getInstance();
            hkMemoryRouter::alignedFree(*router->m_heap, m_buf);
        }
    }

    // IDA: ?flushBuffer@hkBufferedStreamWriter@@IEAAHXZ @ 0x1407a82a0
    int flushBuffer() {
        if (!m_stream) {
            return 0;
        }
        int written = 0;
        if (m_bufSize > 0) {
            while (true) {
                int bytesWritten = m_stream->write(&m_buf[written], m_bufSize - written);
                written += bytesWritten;
                if (!bytesWritten) {
                    break;
                }
                if (written >= m_bufSize) {
                    break;
                }
            }
        }
        m_bufSize = 0;
        return written;
    }

    // IDA: ?write@hkBufferedStreamWriter@@UEAAHPEBXH@Z @ 0x1407a8310
    int write(const void* mem, int memSize) override {
        unsigned int remaining = memSize;
        int avail = m_bufCapacity - m_bufSize;
        const char* memPtr = static_cast<const char*>(mem);

        if (memSize <= avail) {
            hkString::memCpy(&m_buf[m_bufSize], memPtr, memSize);
            m_bufSize += memSize;
        } else {
            while (true) {
                hkString::memCpy(&m_buf[m_bufSize], memPtr, avail);
                m_bufSize += avail;
                remaining -= avail;
                memPtr += avail;
                if (static_cast<unsigned int>(flushBuffer()) != m_bufSize) {
                    break;
                }
                avail = m_bufCapacity - m_bufSize;
                if (remaining <= static_cast<unsigned int>(avail)) {
                    hkString::memCpy(&m_buf[m_bufSize], memPtr, remaining);
                    m_bufSize += remaining;
                    break;
                }
            }
            return memSize - remaining;
        }
        return memSize;
    }

    // IDA: ?flush@hkBufferedStreamWriter@@UEAAXXZ @ 0x1407a83c0
    void flush() override {
        flushBuffer();
        if (m_stream) {
            m_stream->flush();
        }
    }

    // IDA: ?isOk@hkBufferedStreamWriter@@UEBA?AVhkBool@@XZ @ 0x1407a83f0
    bool isOk() const override {
        if (m_stream) {
            return m_stream->isOk();
        } else {
            return (m_bufSize != m_bufCapacity);
        }
    }

    // IDA: ?seekTellSupported@hkBufferedStreamWriter@@UEBA?AVhkBool@@XZ @ 0x1407a8450
    hkBool seekTellSupported() const override {
        hkBool result;
        if (m_stream) {
            result.m_bool = m_stream->seekTellSupported().m_bool;
        } else {
            result.m_bool = true;
        }
        return result;
    }

    // IDA: ?seek@hkBufferedStreamWriter@@UEAA?AUhkResult@@HW4SeekWhence@hkStreamWriter@@@Z @ 0x1407a84a0
    hkResult seek(int relOffset, int whence) override {
        hkResult result;
        if (!m_stream) {
            int newPos = -1;
            if (whence == 0) {  // SEEK_WHENCE_SET
                newPos = relOffset;
            } else if (whence == 1) {  // SEEK_WHENCE_CUR
                newPos = relOffset + m_bufSize;
            } else if (whence == 2) {  // SEEK_WHENCE_END
                newPos = m_bufSize - relOffset;
            }

            int res = HK_SUCCESS;
            if (newPos < 0) {
                newPos = 0;
                res = 1;  // HK_FAILURE
            } else if (newPos > m_bufCapacity) {
                newPos = m_bufCapacity;
                res = 1;  // HK_FAILURE
            }
            m_bufSize = newPos;
            result.m_result = res;
        } else {
            flushBuffer();
            m_stream->seek(relOffset, whence);
            result.m_result = HK_SUCCESS;
        }
        return result;
    }

    // IDA: ?tell@hkBufferedStreamWriter@@UEBAHXZ @ 0x1407a8540
    int tell() const override {
        int streamPos = 0;
        if (m_stream) {
            streamPos = m_stream->tell();
            if (streamPos < 0) {
                return -1;
            }
        }
        return streamPos + m_bufSize;
    }
};

// hkMemoryTrack - Memory track for sector-based storage
// IDA: ??0hkMemoryTrack@@QEAA@H@Z @ 0x1407b2220
class hkMemoryTrack {
public:
    hkArrayBase<unsigned char*> m_sectors;
    int m_numBytesPerSector;
    int m_numBytesLastSector;
    int m_numBytesRead;
    int m_numSectorsUnloaded;

    // IDA: ??0hkMemoryTrack@@QEAA@H@Z @ 0x1407b2220
    hkMemoryTrack(int numBytesPerSector) {
        m_sectors.m_data = nullptr;
        m_sectors.m_size = 0;
        m_sectors.m_capacityAndFlags = 0x80000000;
        m_numBytesRead = 0;
        m_numSectorsUnloaded = 0;
        m_numBytesLastSector = numBytesPerSector;
        m_numBytesPerSector = numBytesPerSector;
    }

    // IDA: ??1hkMemoryTrack@@QEAA@XZ @ 0x1407b2550
    ~hkMemoryTrack() {
        clear();
        int cap = m_sectors.m_capacityAndFlags;
        m_sectors.m_size = 0;
        if (cap >= 0) {
            hkContainerHeapAllocator::s_alloc.bufFree(m_sectors.m_data, 8 * cap);
        }
        m_sectors.m_data = nullptr;
        m_sectors.m_capacityAndFlags = 0x80000000;
    }

    // IDA: ?clear@hkMemoryTrack@@QEAAXXZ @ 0x1407b2250
    void clear() {
        for (int i = 0; i < m_sectors.m_size; i++) {
            unsigned char* sector = m_sectors.m_data[i];
            hkMemoryRouter* router = hkMemoryRouter::getInstance();
            router->m_heap->blockFree(sector, m_numBytesPerSector);
        }
        int cap = m_sectors.m_capacityAndFlags;
        m_sectors.m_size = 0;
        if (cap >= 0) {
            hkContainerHeapAllocator::s_alloc.bufFree(m_sectors.m_data, 8 * cap);
        }
        m_sectors.m_data = nullptr;
        m_sectors.m_capacityAndFlags = 0x80000000;
        m_numBytesLastSector = m_numBytesPerSector;
        m_numBytesRead = 0;
        m_numSectorsUnloaded = 0;
    }

    // IDA: ?write@hkMemoryTrack@@QEAAXPEBXH@Z @ 0x1407b20f0
    void write(const void* data, int numBytes) {
        if (numBytes <= 0) return;

        int remaining = numBytes;
        const char* dataPtr = static_cast<const char*>(data);
        int sectorIdx = m_sectors.m_size - 1;

        while (remaining > 0) {
            int spaceInCurrentSector = m_numBytesPerSector - m_numBytesLastSector;

            // Need to allocate new sector
            if (m_numBytesPerSector == m_numBytesLastSector) {
                hkMemoryRouter* router = hkMemoryRouter::getInstance();
                unsigned char* newSector = static_cast<unsigned char*>(
                    router->m_heap->blockAlloc(m_numBytesPerSector));

                if (m_sectors.m_size == (m_sectors.m_capacityAndFlags & 0x3FFFFFFF)) {
                    hkArrayUtil::_reserveMore(hkContainerHeapAllocator::s_alloc,
                        reinterpret_cast<void**>(&m_sectors.m_data), 8);
                }
                m_sectors.m_data[m_sectors.m_size++] = newSector;
                sectorIdx++;
                spaceInCurrentSector = m_numBytesPerSector;
                m_numBytesLastSector = 0;
            }

            unsigned char* dest = &m_sectors.m_data[sectorIdx][m_numBytesLastSector];
            if (remaining <= spaceInCurrentSector) {
                hkString::memCpy(dest, dataPtr, remaining);
                m_numBytesLastSector += remaining;
                break;
            } else {
                hkString::memCpy(dest, dataPtr, spaceInCurrentSector);
                remaining -= spaceInCurrentSector;
                dataPtr += spaceInCurrentSector;
                m_numBytesLastSector = m_numBytesPerSector;
            }
        }
    }

    // IDA: ?appendByMove@hkMemoryTrack@@QEAAXPEAV1@@Z @ 0x1407b2320
    void appendByMove(hkMemoryTrack* other) {
        int otherSize = other->m_sectors.m_size;

        // Move all complete sectors
        for (int i = 0; i < otherSize - 1; i++) {
            char* sector = reinterpret_cast<char*>(other->m_sectors.m_data[i]);
            write(sector, other->m_numBytesPerSector);
            hkMemoryRouter* router = hkMemoryRouter::getInstance();
            router->m_heap->blockFree(sector, other->m_numBytesPerSector);
        }

        // Move last sector (may be partial)
        if (otherSize > 0) {
            char* lastSector = reinterpret_cast<char*>(other->m_sectors.m_data[otherSize - 1]);
            write(lastSector, other->m_numBytesLastSector);
            hkMemoryRouter* router = hkMemoryRouter::getInstance();
            router->m_heap->blockFree(lastSector, other->m_numBytesPerSector);
        }

        // Clear the other track
        int cap = other->m_sectors.m_capacityAndFlags;
        other->m_sectors.m_size = 0;
        if (cap >= 0) {
            hkContainerHeapAllocator::s_alloc.bufFree(other->m_sectors.m_data, 8 * cap);
        }
        other->m_sectors.m_data = nullptr;
        other->m_sectors.m_capacityAndFlags = 0x80000000;
        other->clear();
    }

    // IDA: ?read@hkMemoryTrack@@QEAAXPEAXH@Z @ 0x1407b1cf0
    void read(void* data, int numBytes) {
        if (numBytes <= 0) return;

        char* dataPtr = static_cast<char*>(data);
        int sectorIdx = m_numBytesRead / m_numBytesPerSector - m_numSectorsUnloaded;
        int offsetInSector = m_numBytesRead % m_numBytesPerSector;
        int remaining = numBytes;

        for (int i = sectorIdx; remaining > 0; i++) {
            int sectorEnd = (i >= m_sectors.m_size - 1) ? m_numBytesLastSector : m_numBytesPerSector;
            int availableInSector = sectorEnd - offsetInSector;
            unsigned char* src = &m_sectors.m_data[i][offsetInSector];

            if (remaining <= availableInSector) {
                hkString::memCpy(dataPtr, src, remaining);
                m_numBytesRead += remaining;
                break;
            } else {
                hkString::memCpy(dataPtr, src, availableInSector);
                m_numBytesRead += availableInSector;
                remaining -= availableInSector;
                dataPtr += availableInSector;
                sectorIdx++;
                offsetInSector = 0;
            }
        }
    }

    // IDA: ?unloadReadSectors@hkMemoryTrack@@QEAAXXZ @ 0x1407b1df0
    void unloadReadSectors() {
        int sectorsToUnload = (m_numBytesRead - m_numSectorsUnloaded * m_numBytesPerSector) / m_numBytesPerSector;

        for (int i = 0; i < sectorsToUnload; i++) {
            unsigned char* sector = m_sectors.m_data[0];
            hkMemoryRouter* router = hkMemoryRouter::getInstance();
            router->m_heap->blockFree(sector, m_numBytesPerSector);

            // Shift remaining sectors
            m_sectors.m_size--;
            for (int j = 0; j < m_sectors.m_size; j++) {
                m_sectors.m_data[j] = m_sectors.m_data[j + 1];
            }
            m_numSectorsUnloaded++;
        }
    }
};

// hkMemoryTrackStreamWriter - Stream writer backed by hkMemoryTrack
// IDA: ??1hkMemoryTrackStreamWriter@@UEAA@XZ @ 0x1407a81b0
class hkMemoryTrackStreamWriter : public hkStreamWriter {
public:
    enum TrackOwnership {
        TRACK_BORROW = 0,  // Caller owns the track
        TRACK_TAKE = 1     // Stream writer owns the track
    };

    hkMemoryTrack* m_track;
    TrackOwnership m_ownerShip;

    // IDA: ?isOk@hkMemoryTrackStreamWriter@@UEBA?AVhkBool@@XZ @ 0x14079d000
    bool isOk() const override {
        return true;
    }

    // IDA: ?tell@hkMemoryTrackStreamWriter@@UEBAHXZ @ 0x1407a8190
    int tell() const override {
        return m_track->m_numBytesLastSector +
               m_track->m_numBytesPerSector * (m_track->m_sectors.m_size + m_track->m_numSectorsUnloaded - 1);
    }

    // IDA: ?write@hkMemoryTrackStreamWriter@@UEAAHPEBXH@Z @ 0x1407b2200
    int write(const void* mem, int size) override {
        m_track->write(mem, size);
        return size;
    }

    // IDA: ?clear@hkMemoryTrackStreamWriter@@UEAAXXZ @ 0x1407b2450
    void clear() {
        m_track->clear();
    }

    // IDA: ??1hkMemoryTrackStreamWriter@@UEAA@XZ @ 0x1407a81b0
    ~hkMemoryTrackStreamWriter() override {
        if (m_ownerShip == TRACK_TAKE && m_track) {
            delete m_track;
            m_track = nullptr;
        }
    }
};

// hkMemoryTrackStreamReader - Stream reader backed by hkMemoryTrack
// IDA: ??0hkMemoryTrackStreamReader@@QEAA@PEBVhkMemoryTrack@@W4MemoryType@0@_N@Z @ 0x1407b1db0
class hkMemoryTrackStreamReader : public hkStreamReader {
public:
    enum MemoryType {
        MEMORY_BORROW = 0,  // Caller owns the track
        MEMORY_TAKE = 1     // Stream reader owns the track
    };

    const hkMemoryTrack* m_track;
    MemoryType m_memType;
    bool m_unloadSectorsAfterRead;
    int m_overflowOffset;

    // IDA: ??0hkMemoryTrackStreamReader@@QEAA@PEBVhkMemoryTrack@@W4MemoryType@0@_N@Z @ 0x1407b1db0
    hkMemoryTrackStreamReader(const hkMemoryTrack* track, MemoryType t, bool unloadSectorsAfterRead) {
        m_memType = t;
        m_track = track;
        m_memSizeAndFlags = 0xFFFFFFFF;
        m_overflowOffset = -1;
        m_referenceCount = 1;
        m_unloadSectorsAfterRead = unloadSectorsAfterRead;
    }

    // IDA: ??1hkMemoryTrackStreamReader@@UEAA@XZ @ 0x1407b25b0
    ~hkMemoryTrackStreamReader() override {
        if (m_memType == MEMORY_TAKE && m_track) {
            delete const_cast<hkMemoryTrack*>(m_track);
            m_track = nullptr;
        }
    }

    // IDA: ?isOk@hkMemoryTrackStreamReader@@UEBA?AVhkBool@@XZ @ 0x1407b2640
    bool isOk() const override {
        int totalSize = m_track->m_numBytesLastSector +
            m_track->m_numBytesPerSector * (m_track->m_sectors.m_size + m_track->m_numSectorsUnloaded - 1);
        return totalSize > m_overflowOffset;
    }

    // IDA: ?skip@hkMemoryTrackStreamReader@@UEAAHH@Z @ 0x1407b1cd0
    int skip(int nbytes) override {
        return 0;
    }

    // IDA: ?read@hkMemoryTrackStreamReader@@UEAAHPEAXH@Z @ 0x1407b2460
    int read(void* buf, int nbytes) override {
        if (!isOk()) {
            return 0;
        }

        int totalSize = m_track->m_numBytesLastSector +
            m_track->m_numBytesPerSector * (m_track->m_sectors.m_size + m_track->m_numSectorsUnloaded - 1);
        int available = totalSize - m_track->m_numBytesRead;

        if (nbytes < available) {
            available = nbytes;
        }

        // Check for overflow
        if (nbytes > totalSize - m_track->m_numBytesRead) {
            m_overflowOffset = nbytes + m_track->m_numBytesRead;
            return 0;
        }

        const_cast<hkMemoryTrack*>(m_track)->read(buf, available);

        if (m_unloadSectorsAfterRead) {
            const_cast<hkMemoryTrack*>(m_track)->unloadReadSectors();

            // Check if all data has been read
            int newTotal = m_track->m_numBytesLastSector +
                m_track->m_numBytesPerSector * (m_track->m_sectors.m_size + m_track->m_numSectorsUnloaded - 1);
            if (m_track->m_numBytesRead == newTotal) {
                const_cast<hkMemoryTrack*>(m_track)->clear();
                m_overflowOffset = -1;
            }
        }

        return available;
    }
};

// hkCrcStreamWriter - CRC calculating stream writer
// IDA: Template instantiation at 0x140791310-0x140791390
template<typename CrcType, CrcType InitialValue>
class hkCrcStreamWriter : public hkStreamWriter {
public:
    CrcType m_crc;

    hkCrcStreamWriter() : m_crc(InitialValue) {}

    // IDA: ?getCrc@?$hkCrcStreamWriter@I$0ONLIIDCA@@@QEBAIXZ @ 0x140791310
    CrcType getCrc() const {
        return ~m_crc;
    }

    // IDA: ?write@?$hkCrcStreamWriter@I$0ONLIIDCA@@@UEAAHPEBXH@Z @ 0x140791320
    int write(const void* buf, int nbytes) override {
        const unsigned char* ptr = static_cast<const unsigned char*>(buf);
        for (int i = 0; i < nbytes; i++) {
            unsigned int v6 = static_cast<unsigned char>(m_crc ^ ptr[i]);
            for (int bit = 0; bit < 8; bit++) {
                if (v6 & 1) {
                    v6 = (v6 >> 1) ^ 0xEDB88320;
                } else {
                    v6 >>= 1;
                }
            }
            m_crc = v6 ^ (m_crc >> 8);
        }
        return nbytes;
    }
};

// hkRefCountedProperties - Reference counted properties container
// IDA: Multiple functions at 0x14079dxxx
class hkRefCountedProperties : public hkReferencedObject {
public:
    // Entry structure for property storage
    struct Entry {
        unsigned short m_key;
        unsigned short m_padding;
        hkRefPtr<hkReferencedObject> m_object;
    };

    // Reference count handling enum
    enum ReferenceCountHandling {
        REFERENCE_COUNT_INCREMENT,
        REFERENCE_COUNT_NONE
    };

    hkArrayBase<Entry> m_entries;

    // IDA: ??0hkRefCountedProperties@@QEAA@XZ @ 0x14079e090
    hkRefCountedProperties() {
        m_memSizeAndFlags = 0xFFFFFFFF;
        m_referenceCount = 1;
        m_entries.m_capacityAndFlags = 0x80000000;
        m_entries.m_data = nullptr;
        m_entries.m_size = 0;
    }

    // IDA: ??0hkRefCountedProperties@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x14079e0d0
    hkRefCountedProperties(hkFinishLoadedObjectFlag flag) {
        // Serialization constructor - skip initialization
    }

    // IDA: ?accessProperty@hkRefCountedProperties@@QEBAPEAVhkReferencedObject@@G@Z @ 0x14079de60
    hkReferencedObject* accessProperty(unsigned short propertyKey) const {
        int idx = m_entries.m_size - 1;
        if (idx < 0) return nullptr;
        for (int i = idx; i >= 0; i--) {
            if (m_entries.m_data[i].m_key == propertyKey) {
                return m_entries.m_data[i].m_object.m_ptr;
            }
        }
        return nullptr;
    }

    // IDA: ?replaceProperty@hkRefCountedProperties@@QEAAXGPEAVhkReferencedObject@@@Z @ 0x14079dd50
    void replaceProperty(unsigned short propertyKey, hkReferencedObject* newPropertyObject);

    // IDA: ?addPropertyInternal@hkRefCountedProperties@@IEAAXGPEAVhkReferencedObject@@W4ReferenceCountHandling@1@@Z @ 0x14079deb0
    void addPropertyInternal(unsigned short propertyKey, hkReferencedObject* propertyObject, ReferenceCountHandling referenceHandling);

    // IDA: ?removeProperty@hkRefCountedProperties@@QEAAXG@Z @ 0x14079dfc0
    void removeProperty(unsigned short propertyKey);
};

// hkStringPtr - String pointer with optional ownership
// The lowest bit of m_stringAndFlag indicates if the string is owned (allocated)
// IDA: ??0hkStringPtr@@QEAA@XZ @ 0x14079d8f0
class hkStringPtr {
public:
    const char* m_stringAndFlag;  // Lowest bit is ownership flag

    // IDA: ??0hkStringPtr@@QEAA@XZ @ 0x14079d8f0
    hkStringPtr() : m_stringAndFlag(nullptr) {}

    // IDA: ??0hkStringPtr@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x14079d910
    hkStringPtr(hkFinishLoadedObjectFlag f) : m_stringAndFlag(nullptr) {}

    // IDA: ??0hkStringPtr@@QEAA@PEBD@Z @ 0x14079daa0
    hkStringPtr(const char* string) : m_stringAndFlag(nullptr) {
        assign(string, -1);
    }

    // IDA: ??0hkStringPtr@@QEAA@PEBDH@Z @ 0x14079dad0
    hkStringPtr(const char* string, int len) : m_stringAndFlag(nullptr) {
        assign(string, len);
    }

    // IDA: ??0hkStringPtr@@QEAA@AEBV0@@Z @ 0x14079db00
    hkStringPtr(const hkStringPtr& other) : m_stringAndFlag(nullptr) {
        assign(other.cString(), -1);
    }

    // IDA: ??1hkStringPtr@@QEAA@XZ @ 0x14079db30
    ~hkStringPtr() {
        assign(nullptr, -1);
    }

    // IDA: ??4hkStringPtr@@QEAAAEAV0@PEBD@Z @ 0x14079db50
    hkStringPtr& operator=(const char* string) {
        assign(string, -1);
        return *this;
    }

    // IDA: ??4hkStringPtr@@QEAAAEAV0@AEBV0@@Z @ 0x14079db80
    hkStringPtr& operator=(const hkStringPtr& other) {
        assign(other.cString(), -1);
        return *this;
    }

    // Get the string pointer (clears the ownership flag)
    const char* cString() const {
        return reinterpret_cast<const char*>(reinterpret_cast<uintptr_t>(m_stringAndFlag) & ~1ULL);
    }

    // Check if string is owned
    bool isOwned() const {
        return (reinterpret_cast<uintptr_t>(m_stringAndFlag) & 1) != 0;
    }

    // IDA: ?getLength@hkStringPtr@@QEBAHXZ @ 0x14079d920
    int getLength() const {
        const char* str = cString();
        if (str) {
            return hkString::strLen(str);
        }
        return 0;
    }

    // IDA: ?set@hkStringPtr@@QEAAXPEBDH@Z @ 0x14079dbb0
    void set(const char* string, int len = -1) {
        assign(string, len);
    }

    // IDA: ?setPointerAligned@hkStringPtr@@QEAAXPEBD@Z @ 0x14079dbc0
    void setPointerAligned(const char* s) {
        if (cString() != s) {
            assign(nullptr, -1);
            m_stringAndFlag = s;
        }
    }

    // IDA: ?printf@hkStringPtr@@QEAAXPEBDZZ @ 0x14079dc00
    // Variable arguments printf - stub implementation
    void printf(const char* fmt, ...) {
        // Stub - would format string and assign
    }

    // Assignment function - core implementation
    // IDA: assign @ 0x14079da00
    void assign(const char* src, int len) {
        const char* currentStr = cString();
        bool currentlyOwned = isOwned();

        if (currentStr != src) {
            // Free existing string if owned
            if (currentlyOwned && currentStr) {
                // IDA calls hkMemoryRouter::easyFree
                // For stub, just free the memory
                ::operator delete(const_cast<char*>(currentStr));
            }

            if (src) {
                // Allocate new string
                char* newStr;
                if (len == -1) {
                    len = static_cast<int>(strlen(src));
                }
                newStr = static_cast<char*>(::operator new(len + 1));
                memcpy(newStr, src, len);
                newStr[len] = '\0';
                // Set with ownership flag
                m_stringAndFlag = reinterpret_cast<const char*>(reinterpret_cast<uintptr_t>(newStr) | 1);
            } else {
                m_stringAndFlag = nullptr;
            }
        }
    }

    // Static operator= for external use
    static void operatorAssign(const char** ptr, const char* src, int len);
};

// hkOstream inline implementations (after hkStringPtr definition)
// IDA: ??6hkOstream@@QEAAAEAV0@AEBVhkStringPtr@@@Z @ 0x1407a79c0
inline hkOstream& hkOstream::operator<<(const hkStringPtr& str) {
    const char* s = str.cString();
    if (s) {
        unsigned int len = str.getLength();
        m_writer.m_ptr->write(s, len);
    } else {
        m_writer.m_ptr->write("(null)", 6);
    }
    return *this;
}

// hkStringBuf - String buffer with inline storage
// Uses hkArray-like structure with 128-byte inline storage
// IDA: ??0hkStringBuf@@QEAA@XZ @ 0x1407a6b80
class hkStringBuf {
public:
    // ReplaceType - Replacement mode for replace operations
    // IDA: enum in hkStringBuf
    enum ReplaceType {
        REPLACE_ONE,
        REPLACE_ALL
    };

    // Internal array structure
    char* m_data;
    int m_size;
    int m_capacityAndFlags;
    char m_storage[128];  // Inline storage buffer

    // IDA: ??0hkStringBuf@@QEAA@XZ @ 0x1407a6b80
    hkStringBuf() {
        m_size = 0;
        m_capacityAndFlags = 0x80000020;  // 0x80000000 | 32 (initial capacity)
        m_data = m_storage;
    }

    // IDA: ??0hkStringBuf@@QEAA@PEBD@Z @ 0x1407a6bb0
    hkStringBuf(const char* s) {
        m_size = 0;
        m_capacityAndFlags = 0x80000020;
        m_data = m_storage;
        if (s) {
            unsigned int len = hkString::strLen(s);
            setLength(len);
            hkString::memCpy(m_data, s, len);
        } else {
            m_size = 1;
            m_storage[0] = 0;
        }
    }

    // IDA: ??0hkStringBuf@@QEAA@AEBVhkStringPtr@@@Z @ 0x1407a6c40
    hkStringBuf(const hkStringPtr& s) {
        m_size = 0;
        m_capacityAndFlags = 0x80000020;
        m_data = m_storage;
        const char* str = s.cString();
        if (str) {
            operator=(str);
        }
    }

    // IDA: ??0hkStringBuf@@QEAA@AEBV0@@Z @ 0x1407a6d80
    hkStringBuf(const hkStringBuf& s) {
        m_size = 0;
        m_capacityAndFlags = 0x80000020;
        m_data = m_storage;
        if (s.m_size > 128) {
            int n = s.m_size;
            m_data = static_cast<char*>(hkContainerTempAllocator::s_alloc.bufAlloc(n));
            m_capacityAndFlags = n;
        }
        m_size = s.m_size;
        if (m_size > 0) {
            hkString::memCpy(m_data, s.m_data, m_size);
        }
    }

    // IDA: ??1hkStringBuf@@QEAA@XZ
    ~hkStringBuf() {
        if (m_capacityAndFlags >= 0 && m_data != m_storage) {
            hkContainerTempAllocator::s_alloc.bufFree(m_data, m_capacityAndFlags & 0x3FFFFFFF);
        }
    }

    // IDA: ??4hkStringBuf@@QEAAAEAV0@AEBV0@@Z @ 0x1407a6a50
    hkStringBuf& operator=(const hkStringBuf& s) {
        if ((m_capacityAndFlags & 0x3FFFFFFF) < s.m_size) {
            if (m_capacityAndFlags >= 0 && m_data != m_storage) {
                hkContainerTempAllocator::s_alloc.bufFree(m_data, m_capacityAndFlags & 0x3FFFFFFF);
            }
            int n = s.m_size;
            m_data = static_cast<char*>(hkContainerTempAllocator::s_alloc.bufAlloc(n));
            m_capacityAndFlags = n;
        }
        m_size = s.m_size;
        if (m_size > 0) {
            hkString::memCpy(m_data, s.m_data, m_size);
        }
        return *this;
    }

    // setLength - allocate or resize buffer
    // IDA: ?setLength@hkStringBuf@@AEAAXH@Z @ 0x14079c620
    // setLength - allocate or resize buffer (private method)
    void setLength(int length) {
        int newSize = length + 1;  // +1 for null terminator
        int capacity = m_capacityAndFlags & 0x3FFFFFFF;
        if (capacity < newSize) {
            int newCap = capacity * 2;
            if (newSize < newCap) {
                newCap = newSize;
            }
            hkArrayUtil::_reserve(hkContainerTempAllocator::s_alloc,
                reinterpret_cast<void**>(this), newCap, 1);
        }
        m_size = newSize;
        m_data[length] = 0;  // Set null terminator
    }

    // Accessors
    const char* cString() const { return m_data; }
    char* data() { return m_data; }
    int length() const { return m_size; }

    // IDA: ?indexOf@hkStringBuf@@QEBAHDHH@Z @ 0x1407a5bb0
    int indexOf(char c, int startIndex = 0, int endIndex = 0x7FFFFFFF) const {
        int end = m_size - 1;
        if (endIndex < end) {
            end = endIndex;
        }
        if (startIndex >= end) {
            return -1;
        }
        for (int i = startIndex; i < end; i++) {
            if (m_data[i] == c) {
                return i;
            }
        }
        return -1;
    }

    // IDA: ?compareTo@hkStringBuf@@QEBAHPEBD@Z @ 0x1407a5d90
    int compareTo(const char* other) const {
        return hkString::strCmp(m_data, other);
    }

    // IDA: ??8hkStringBuf@@QEBAIPEBD@Z @ 0x1407a5de0
    bool operator==(const char* other) const {
        return compareTo(other) == 0;
    }

    // IDA: ?indexOf@hkStringBuf@@QEBAHPEBDHH@Z @ 0x1407a5bf0
    int indexOf(const char* s, int startIndex = 0, int endIndex = 0x7FFFFFFF) const {
        const char* found = hkString::strStr(&m_data[startIndex], s);
        if (found) {
            return (int)(found - m_data);
        }
        return -1;
    }

    // IDA: ?indexOfCase@hkStringBuf@@QEBAHPEBD@Z @ 0x1407a5c20
    int indexOfCase(const char* needle) const {
        if (!m_data || !m_data[0]) {
            return -1;
        }
        const char* p = m_data;
        int idx = 0;
        while (*p) {
            int i = 0;
            if (needle[i]) {
                char c1 = needle[i];
                char c2 = p[i];
                // Case-insensitive comparison
                if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
                if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
                while (c1 == c2 && needle[i]) {
                    i++;
                    c1 = needle[i];
                    c2 = p[i];
                    if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
                    if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
                }
            }
            if (!needle[i]) {
                return idx;
            }
            p++;
            idx++;
        }
        return -1;
    }

    // IDA: ?lastIndexOf@hkStringBuf@@QEBAHDHH@Z @ 0x1407a5ce0
    int lastIndexOf(char c, int start = 0, int end = 0x7FFFFFFF) const {
        if (end > m_size - 1) {
            end = m_size - 1;
        }
        int i = end - 1;
        if (i < start) {
            return -1;
        }
        while (m_data[i] != c) {
            i--;
            if (i < start) {
                return -1;
            }
        }
        return i;
    }

    // IDA: ?lastIndexOf@hkStringBuf@@QEBAHPEBDHH@Z @ 0x1407a5d20
    int lastIndexOf(const char* needle, int startIndex = 0, int endIndex = 0x7FFFFFFF) const {
        const char* found = hkString::strStr(m_data, needle);
        if (!found) {
            return -1;
        }
        unsigned int lastPos = 0;
        do {
            lastPos = (unsigned int)(found - m_data);
            found = hkString::strStr(found + 1, needle);
        } while (found);
        return lastPos;
    }

    // IDA: ?compareToIgnoreCase@hkStringBuf@@QEBAHPEBD@Z @ 0x1407a5da0
    int compareToIgnoreCase(const char* other) const {
        return hkString::strCasecmp(m_data, other);
    }

    // IDA: ?startsWith@hkStringBuf@@QEBAIPEBD@Z @ 0x1407a5e00
    bool startsWith(const char* s) const {
        if (!m_data || !m_data[0]) {
            return false;
        }
        int i = 0;
        while (s[i]) {
            if (m_data[i] != s[i]) {
                return false;
            }
            i++;
            if (!m_data[i]) {
                break;
            }
        }
        return s[i] == 0;
    }

    // IDA: ?startsWithCase@hkStringBuf@@QEBAIPEBD@Z @ 0x1407a5e50
    bool startsWithCase(const char* s) const {
        if (!m_data || !m_data[0]) {
            return s == nullptr || s[0] == 0;
        }
        int i = 0;
        while (s[i]) {
            char c1 = m_data[i];
            char c2 = s[i];
            if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
            if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
            if (c1 != c2) {
                return false;
            }
            i++;
            if (!m_data[i]) {
                return true;
            }
        }
        return true;
    }

    // IDA: ?endsWith@hkStringBuf@@QEBAIPEBD@Z @ 0x1407a5eb0
    bool endsWith(const char* s) const {
        int sLen = hkString::strLen(s);
        int thisLen = m_size;
        if (sLen > thisLen - 1) {
            return false;
        }
        for (int i = 0; i < sLen; i++) {
            if (m_data[thisLen - sLen - 1 + i] != s[i]) {
                return false;
            }
        }
        return true;
    }

    // IDA: ?endsWithCase@hkStringBuf@@QEBAIPEBD@Z @ 0x1407a5f20
    bool endsWithCase(const char* s) const {
        int sLen = hkString::strLen(s);
        int thisLen = m_size;
        if (sLen > thisLen - 1) {
            return false;
        }
        for (int i = 0; i < sLen; i++) {
            char c1 = m_data[thisLen - sLen - 1 + i];
            char c2 = s[i];
            if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
            if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
            if (c1 != c2) {
                return false;
            }
        }
        return true;
    }

    // IDA: ?lowerCase@hkStringBuf@@QEAAXXZ @ 0x1407a6020
    void lowerCase() {
        for (int i = 0; i < m_size - 1; i++) {
            char c = m_data[i];
            if (c >= 'A' && c <= 'Z') {
                c += 32;
            }
            m_data[i] = c;
        }
    }

    // IDA: ?upperCase@hkStringBuf@@QEAAXXZ @ 0x1407a6080
    void upperCase() {
        for (int i = 0; i < m_size - 1; i++) {
            char c = m_data[i];
            if (c >= 'a' && c <= 'z') {
                c -= 32;
            }
            m_data[i] = c;
        }
    }

    // IDA: ?chompStart@hkStringBuf@@QEAAXH@Z @ 0x1407a60e0
    void chompStart(int n) {
        int toRemove = m_size - 1;
        if (n < toRemove) {
            toRemove = n;
        }
        if (toRemove > 0) {
            m_size -= toRemove;
            int newSize = m_size;
            if (newSize > 0) {
                for (int i = 0; i < newSize; i++) {
                    m_data[i] = m_data[i + toRemove];
                }
            }
        }
    }

    // IDA: ?slice@hkStringBuf@@QEAAXHH@Z @ 0x1407a66c0
    void slice(int startOffset, unsigned int length) {
        if (startOffset) {
            hkMemUtil::memMove(m_data, &m_data[startOffset], length);
        }
        int newSize = length + 1;
        int capacity = m_capacityAndFlags & 0x3FFFFFFF;
        if (capacity < newSize) {
            int newCap = capacity * 2;
            if (newSize < newCap) {
                newCap = newSize;
            }
            hkArrayUtil::_reserve(hkContainerTempAllocator::s_alloc,
                reinterpret_cast<void**>(this), newCap, 1);
        }
        m_size = newSize;
        m_data[length] = 0;
    }

    // IDA: ?pathBasename@hkStringBuf@@QEAAXXZ @ 0x1407a6130
    void pathBasename() {
        int lastSlash = lastIndexOf('/');
        int lastBackslash = lastIndexOf('\\');
        int pos = lastSlash;
        if (lastBackslash > pos) {
            pos = lastBackslash;
        }
        if (pos >= 0) {
            chompStart(pos + 1);
        }
    }

    // IDA: ?pathDirname@hkStringBuf@@QEAAXXZ @ 0x1407a6850
    void pathDirname() {
        int lastSlash = lastIndexOf('/');
        int lastBackslash = lastIndexOf('\\');
        int pos = lastSlash;
        if (lastBackslash > pos) {
            pos = lastBackslash;
        }
        if (pos < 0) {
            clear();
        } else {
            slice(0, pos);
        }
    }

    // IDA: ?pathExtension@hkStringBuf@@QEAAXXZ @ 0x1407a68b0
    void pathExtension() {
        int dotPos = lastIndexOf('.');
        if (dotPos == -1) {
            clear();
        } else {
            chompStart(dotPos);
        }
    }

    // IDA: ?chompEnd@hkStringBuf@@QEAAXH@Z @ 0x1407a6690
    void chompEnd(int n) {
        if (n > 0) {
            int newSize = m_size - n - 1;
            if (newSize < 0) {
                newSize = 0;
            }
            setLength(newSize);
        }
    }

    // IDA: ?set@hkStringBuf@@QEAAXPEBDH@Z @ 0x1407a6740
    void set(const char* s, int len = -1) {
        int slen = len;
        if (len < 0) {
            slen = hkString::strLen(s);
        }
        int newSize = slen + 1;
        int capacity = m_capacityAndFlags & 0x3FFFFFFF;
        if (capacity < newSize) {
            int newCap = capacity * 2;
            if (newSize < newCap) {
                newCap = newSize;
            }
            hkArrayUtil::_reserve(hkContainerTempAllocator::s_alloc,
                reinterpret_cast<void**>(this), newCap, 1);
        }
        m_size = newSize;
        m_data[slen] = 0;
        hkMemUtil::memCpy(m_data, s, slen);
    }

    // IDA: ?append@hkStringBuf@@QEAAXPEBDH@Z @ 0x1407a67e0
    void append(const char* s, int len = -1) {
        if (s) {
            int slen = len;
            if (len < 0) {
                slen = hkString::strLen(s);
            }
            int oldLen = m_size - 1;
            setLength(oldLen + slen);
            hkMemUtil::memCpy(&m_data[oldLen], s, slen);
        }
    }

    // IDA: ??YhkStringBuf@@QEAAAEAV0@PEBD@Z @ 0x1407a6b00
    hkStringBuf& operator+=(const char* other) {
        if (other) {
            int len = hkString::strLen(other);
            // Append the string
            append(other, len);
        }
        return *this;
    }

    // Clear buffer
    void clear() {
        if ((m_capacityAndFlags & 0x3FFFFFFF) == 0) {
            // Need to reserve at least 1 byte
            hkArrayUtil::_reserve(hkContainerTempAllocator::s_alloc,
                reinterpret_cast<void**>(this), 1, 1);
        }
        m_size = 1;
        m_data[0] = 0;
    }

    // IDA: ?getArray@hkStringBuf@@QEAAAEAV?$hkArray@DUhkContainerTempAllocator@@@@XZ @ 0x1407a5b80
    // getArray - Returns this as hkArray<char> reference (same memory layout)
    hkStringBuf& getArray() {
        return *this;
    }

    // IDA: ??MhkStringBuf@@QEBA?AVhkBool@@PEBD@Z @ 0x1407a5db0
    // operator< - Compare with C string, returns hkBool
    hkBool operator<(const char* other) const {
        hkBool result;
        result.m_bool = hkString::strCmp(m_data, other) < 0;
        return result;
    }

    // IDA: ??4hkStringBuf@@QEAAAEAV0@PEBD@Z @ 0x1407a6180
    // operator= - Assignment from C string (extended version with null handling)
    hkStringBuf& operator=(const char* s) {
        if (s) {
            unsigned int len = hkString::strLen(s);
            setLength(len);
            hkString::memCpy(m_data, s, len);
        } else {
            if ((m_capacityAndFlags & 0x3FFFFFFF) == 0) {
                hkArrayUtil::_reserve(hkContainerTempAllocator::s_alloc,
                    reinterpret_cast<void**>(this), 1, 1);
            }
            m_size = 1;
            m_data[0] = 0;
        }
        return *this;
    }

    // IDA: ?replace@hkStringBuf@@QEAAIDDW4ReplaceType@1@@Z @ 0x1407a5fc0
    // replace - Replace single character with another
    unsigned int replace(char from, char to, ReplaceType rt) {
        unsigned int replaced = 0;
        if (m_size - 1 > 0) {
            for (int i = 0; i < m_size - 1; i++) {
                if (m_data[i] == from) {
                    m_data[i] = to;
                    replaced = 1;
                    if (rt == REPLACE_ONE) {
                        break;
                    }
                }
            }
        }
        return replaced;
    }

    // IDA: ?replace@hkStringBuf@@QEAAIPEBD0W4ReplaceType@1@@Z @ 0x1407a7100
    // replace - Replace string with another string
    unsigned int replace(const char* from, const char* to, ReplaceType rtype);

    // IDA: ?split@hkStringBuf@@QEAAHHAEAV?$hkArray@PEBDUhkContainerTempAllocator@@@@@Z @ 0x1407a6230
    // split - Split string by separator into array of C string pointers
    void split(char sep, hkArray<const char*, hkContainerTempAllocator>& bits) {
        bits.pushBack(m_data);
        for (int i = indexOf(sep, 0, 0x7FFFFFFF); i >= 0; i = indexOf(sep, i + 1, 0x7FFFFFFF)) {
            m_data[i] = 0;
            bits.pushBack(&m_data[i + 1]);
        }
    }

    // IDA: ?insert@hkStringBuf@@QEAAXHPEBDH@Z @ 0x1407a6b50
    // insert - Insert string at position
    void insert(int pos, const char* s, int len = -1) {
        if (s) {
            int numtoinsert = len;
            if (len < 0) {
                numtoinsert = hkString::strLen(s);
            }
            // reinterpret as hkArrayBase<char> to call _insertAt
            hkArrayBase<char>* arr = reinterpret_cast<hkArrayBase<char>*>(this);
            arr->_insertAt(&hkContainerTempAllocator::s_alloc, pos, s, numtoinsert);
        }
    }

    // IDA: ?prepend@hkStringBuf@@QEAAXPEBDH@Z @ 0x1407a6e20
    // prepend - Insert string at beginning
    void prepend(const char* s, int len = -1) {
        insert(0, s, len);
    }

    // IDA: ?pathNormalize@hkStringBuf@@QEAAXXZ @ 0x1407a6e40
    // pathNormalize - Normalize path separators and resolve . and ..
    void pathNormalize();

    // IDA: ?pathAppend@hkStringBuf@@QEAAAEAV1@PEBD00@Z @ 0x1407a68f0
    // pathAppend - Append path components with proper separator handling
    hkStringBuf& pathAppend(const char* p0, const char* p1 = nullptr, const char* p2 = nullptr);

    // IDA: ??0hkStringBuf@@QEAA@PEBD00000@Z @ 0x1407a6c80
    // Constructor with multiple strings joined
    hkStringBuf(const char* s0, const char* s1, const char* s2, const char* s3, const char* s4, const char* s5) {
        m_size = 0;
        m_capacityAndFlags = 0x80000020;
        m_data = m_storage;
        m_storage[0] = 0;
        m_size = 1;
        appendJoin(s0, s1, s2, s3, s4, s5);
    }

    // IDA: ??0hkStringBuf@@QEAA@PEBDH@Z @ 0x1407a6ce0
    // Constructor with buffer and length
    hkStringBuf(const char* b, int len) {
        m_size = 0;
        m_data = m_storage;
        m_capacityAndFlags = 0x80000020;
        if (len + 1 > 128) {
            int capacity = len + 1;
            if (capacity < 256) {
                capacity = 256;
            }
            hkArrayUtil::_reserve(hkContainerTempAllocator::s_alloc,
                reinterpret_cast<void**>(this), capacity, 1);
        }
        m_size = len + 1;
        m_data[len] = 0;
        hkString::memCpy(m_data, b, len);
    }

    // IDA: ?printf@hkStringBuf@@QEAAXPEBDZZ @ 0x1407a63a0
    // printf - Format and set string
    void printf(const char* fmt, ...);

    // IDA: ?appendPrintf@hkStringBuf@@QEAAXPEBDZZ @ 0x1407a73a0
    // appendPrintf - Format and append to string
    void appendPrintf(const char* fmt, ...);

    // IDA: ?appendJoin@hkStringBuf@@QEAAAEAV1@PEBD00000@Z @ 0x1407a64d0
    // appendJoin - Append multiple strings
    hkStringBuf& appendJoin(const char* s0, const char* s1, const char* s2, const char* s3, const char* s4, const char* s5) {
        int oldLen = m_size - 1;
        int len[6] = {0, 0, 0, 0, 0, 0};
        const char* ptr[7] = {s0, s1, s2, s3, s4, s5, nullptr};
        int totalLen = oldLen;

        // Calculate total length
        int idx = 0;
        for (const char* p = s0; p; p = ptr[++idx]) {
            len[idx] = hkString::strLen(p);
            totalLen += len[idx];
        }

        // Ensure capacity
        int newSize = totalLen + 1;
        int capacity = m_capacityAndFlags & 0x3FFFFFFF;
        if (capacity < newSize) {
            int newCap = capacity * 2;
            if (newSize < newCap) {
                newCap = newSize;
            }
            hkArrayUtil::_reserve(hkContainerTempAllocator::s_alloc,
                reinterpret_cast<void**>(this), newCap, 1);
        }
        m_size = newSize;
        m_data[totalLen] = 0;

        // Copy strings
        idx = 0;
        int pos = oldLen;
        for (const char* p = s0; p; p = ptr[++idx]) {
            hkString::memCpy(&m_data[pos], p, len[idx]);
            pos += len[idx];
        }
        return *this;
    }

    // IDA: ?setJoin@hkStringBuf@@QEAAAEAV1@PEBD00000@Z @ 0x1407a6610
    // setJoin - Clear and join multiple strings
    hkStringBuf& setJoin(const char* s0, const char* s1, const char* s2, const char* s3, const char* s4, const char* s5) {
        clear();
        appendJoin(s0, s1, s2, s3, s4, s5);
        return *this;
    }
};

// hkaiNavMesh - NavMesh type
class hkaiNavMesh {
public:
    void* m_data;

    hkaiNavMesh() : m_data(nullptr) {}

    static hkClass* staticClass() {
        static hkClass cls("hkaiNavMesh");
        return &cls;
    }
};

// ============================================================================
// hkMesh Types - Havok Mesh System
// ============================================================================

// Forward declarations for hkMesh types
class hkMeshShape;
class hkMeshVertexBuffer;
class hkMeshBody;
class hkMeshTexture;
class hkMeshMaterial;
class hkMeshSystem;
class hkMemoryMeshVertexBuffer;
class hkVertexFormat;
struct hkMeshSection;
struct hkMeshBoneIndexMapping;
struct hkMeshSectionCinfo;
struct hkIndexedTransformSetCinfo;

// hkVertexFormat enums and Element - Forward declarations for use in hkMeshVertexBuffer
// These are defined in hkVertexFormat class below
namespace hkVertexFormatDetail {
    enum ComponentUsage {
        USAGE_POSITION = 0,
        USAGE_NORMAL = 1,
        USAGE_COLOR = 2,
        USAGE_TANGENT = 3,
        USAGE_BINORMAL = 4,
        USAGE_TEXTURE_COORD = 5,
        USAGE_BLEND_INDICES = 6,
        USAGE_BLEND_WEIGHTS = 7,
        USAGE_USER = 8,
        USAGE_BLEND_MATRIX_INDEX = 9,
        USAGE_BLEND_WEIGHTS_LAST_IMPLIED = 10
    };

    enum ComponentType {
        TYPE_NONE = 0,
        TYPE_FLOAT = 1,
        TYPE_INT8 = 2,
        TYPE_UINT8 = 3,
        TYPE_INT16 = 4,
        TYPE_UINT16 = 5,
        TYPE_INT32 = 6,
        TYPE_UINT32 = 7,
        TYPE_ARGB32 = 8,
        TYPE_FLOAT32 = 10
    };

    // Element - Vertex element description (8 bytes)
    struct Element {
        hkEnum<ComponentType, unsigned char> m_dataType;
        unsigned char m_numValues;
        unsigned char m_flags;
        hkEnum<ComponentUsage, unsigned char> m_usage;
        unsigned char m_subUsage;
        unsigned char m_pad0;
        unsigned char m_pad1;
        unsigned char m_pad2;

        Element() : m_dataType(TYPE_NONE), m_numValues(0), m_flags(0), m_usage(USAGE_POSITION), m_subUsage(0), m_pad0(0), m_pad1(0), m_pad2(0) {}
    };
}

// hkMeshSection - Mesh section structure
// IDA: size = 48 bytes (estimated from usage)
struct hkMeshSection {
    // Primitive types enum
    enum PrimitiveType {
        PRIMITIVE_TYPE_TRIANGLE_LIST = 0,
        PRIMITIVE_TYPE_TRIANGLE_STRIP = 1,
        PRIMITIVE_TYPE_TRIANGLE_FAN = 2,
        PRIMITIVE_TYPE_LINE_LIST = 3,
        PRIMITIVE_TYPE_LINE_STRIP = 4,
        PRIMITIVE_TYPE_POINT_LIST = 5,
    };

    // Index types enum
    enum IndexType {
        INDEX_TYPE_NONE = 0,
        INDEX_TYPE_UINT16 = 1,
        INDEX_TYPE_UINT32 = 2,
    };

    hkEnum<PrimitiveType, unsigned char> m_primitiveType;
    unsigned char m_padding[3];
    int m_numPrimitives;
    int m_numIndices;
    int m_vertexStartIndex;
    hkEnum<IndexType, unsigned char> m_indexType;
    unsigned char m_padding2[3];
    int m_transformIndex;
    hkMeshVertexBuffer* m_vertexBuffer;
    void* m_indices;
    int m_sectionIndex;
    hkMeshMaterial* m_material;
};

// hkMeshBoneIndexMapping - Bone index mapping structure
// IDA: cleanupLoadedObjecthkMeshBoneIndexMapping frees buffer at offset 0
struct hkMeshBoneIndexMapping {
    int* m_data;
    int m_size;
    int m_capacityAndFlags;
};

// hkIndexedTransformSet - Indexed transform set (forward definition for hkMemoryMeshBody)
class hkIndexedTransformSet : public hkReferencedObject {
public:
    hkArrayBase<hkMatrix4> m_matrices;          // IDA: m_matrices.m_size used in getNumIndexedTransforms
    hkArrayBase<hkMatrix4> m_inverseMatrices;   // IDA: used in getInverseMatrices
    hkArrayBase<hkMeshBoneIndexMapping> m_indexMappings;
    hkArrayBase<short> m_matricesOrder;         // IDA: m_matricesOrder.m_data used in getIndexTransformsOrder

    // IDA: setMatrices @ 0x1407e92d0
    void setMatrices(int startIndex, const hkMatrix4* matrices, int numMatrices);

    // IDA: getMatrices @ 0x1407e9340
    void getMatrices(int startIndex, hkMatrix4* matrices, int numMatrices) const;

    // IDA: getInverseMatrices @ 0x1407e93b0
    void getInverseMatrices(int startIndex, hkMatrix4* matrices, int numMatrices) const;

    // IDA: setInverseMatrices @ 0x1407e9460
    void setInverseMatrices(int startIndex, const hkMatrix4* matrices, int numMatrices);

    // IDA: calculateMatrix @ 0x1407e94d0
    void calculateMatrix(int index, hkMatrix4* matrixOut) const;

    // IDA: Constructor with hkFinishLoadedObjectFlag @ 0x1407ea310
    hkIndexedTransformSet(hkFinishLoadedObjectFlag flag);
};

// hkMeshMaterial - Mesh material
class hkMeshMaterial : public hkReferencedObject {
public:
    // IDA: staticClass @ 0x1407e5d50
    static const hkClass* staticClass();

    // Virtual methods
    virtual void setName(const char* name) {}
    virtual const char* getName() const { return nullptr; }
    virtual int getNumTextures() const { return 0; }
    virtual void getColors(hkVector4* diffuse, hkVector4* ambient, hkVector4* specular, hkVector4* emissive) const {}
    virtual void setColors(const hkVector4& diffuse, const hkVector4& ambient, const hkVector4& specular, const hkVector4& emissive) {}

    // IDA: createCompatibleVertexFormat
    virtual hkResult createCompatibleVertexFormat(const hkVertexFormat& format, hkVertexFormat& compatibleFormat) const;

    // IDA: createCompatibleVertexBuffer @ 0x1407ebb50
    virtual hkMeshVertexBuffer* createCompatibleVertexBuffer(hkMeshVertexBuffer* buffer) const;

    // IDA: isCompatible @ 0x1407ebb70
    virtual bool isCompatible(const hkMeshVertexBuffer* buffer) const { return false; }

    // IDA: equals @ 0x1407ebc50
    virtual bool equals(const hkMeshMaterial* other) const { return this == other; }

    // IDA: getClassType
    virtual const hkClass* getClassType() const { return staticClass(); }
};

// hkMeshTexture - Forward declaration
class hkMeshTexture;

// hkMemoryMeshMaterial - In-memory mesh material
class hkMemoryMeshMaterial : public hkMeshMaterial {
public:
    hkStringPtr m_materialName;                              // Material name
    hkArrayBase<hkRefPtr<hkMeshTexture>> m_textures;         // Array of texture refs
    hkVector4 m_diffuseColor;
    hkVector4 m_ambientColor;
    hkVector4 m_specularColor;
    hkVector4 m_emissiveColor;

    // IDA: setName @ 0x1407ebba0
    void setName(const char* name) override {
        m_materialName = name;
    }

    // IDA: getName @ 0x1407ebbb0
    const char* getName() const override {
        return m_materialName.cString();
    }

    // IDA: getNumTextures @ 0x1407ebbc0
    int getNumTextures() const override {
        return m_textures.m_size;
    }

    // IDA: getTexture @ 0x1407ebc30
    hkMeshTexture* getTexture(int index) const {
        if (index >= 0 && index < m_textures.m_size) {
            return m_textures.m_data[index].m_ptr;
        }
        return nullptr;
    }

    // IDA: getColors @ 0x1407ebbd0
    void getColors(hkVector4* diffuse, hkVector4* ambient, hkVector4* specular, hkVector4* emissive) const override {
#ifdef _WIN32
        diffuse->m_quad = m_diffuseColor.m_quad;
        ambient->m_quad = m_ambientColor.m_quad;
        specular->m_quad = m_specularColor.m_quad;
        emissive->m_quad = m_emissiveColor.m_quad;
#else
        *diffuse = m_diffuseColor;
        *ambient = m_ambientColor;
        *specular = m_specularColor;
        *emissive = m_emissiveColor;
#endif
    }

    // IDA: setColors @ 0x1407ebc00
    void setColors(const hkVector4& diffuse, const hkVector4& ambient, const hkVector4& specular, const hkVector4& emissive) override {
#ifdef _WIN32
        m_diffuseColor.m_quad = diffuse.m_quad;
        m_ambientColor.m_quad = ambient.m_quad;
        m_specularColor.m_quad = specular.m_quad;
        m_emissiveColor.m_quad = emissive.m_quad;
#else
        m_diffuseColor = diffuse;
        m_ambientColor = ambient;
        m_specularColor = specular;
        m_emissiveColor = emissive;
#endif
    }

    // IDA: addTexture @ 0x1407ebd30
    void addTexture(hkMeshTexture* texture);

    // IDA: constructor with name @ 0x1407ebdc0
    hkMemoryMeshMaterial(const char* name);

    // IDA: constructor with hkFinishLoadedObjectFlag @ 0x1407ebe60
    hkMemoryMeshMaterial(hkFinishLoadedObjectFlag flag);

    // IDA: destructor @ 0x1407ebea0
    virtual ~hkMemoryMeshMaterial();

    // IDA: getClassType @ 0x1407ebe90
    const hkClass* getClassType() const override;

    // IDA: createCompatibleVertexFormat @ 0x1407ebb20
    hkResult createCompatibleVertexFormat(const hkVertexFormat& format, hkVertexFormat& compatibleFormat) const override;

    // IDA: createCompatibleVertexBuffer @ 0x1407ebb50
    hkMeshVertexBuffer* createCompatibleVertexBuffer(hkMeshVertexBuffer* buffer) const override;

    // IDA: isCompatible @ 0x1407ebb70
    bool isCompatible(const hkMeshVertexBuffer* buffer) const override;

    // IDA: equals @ 0x1407ebc50
    bool equals(const hkMeshMaterial* other) const override;
};

// hkMeshVertexBuffer - Mesh vertex buffer base class
class hkMeshVertexBuffer : public hkReferencedObject {
public:
    // LockedVertices structure for vertex buffer locking
    struct LockedVertices {
        void* m_vertexData;
        int m_numVertices;
        int m_numBuffers;  // Number of valid buffers in m_buffers array
        int m_stride;

        // Buffer - Per-element buffer info for indexed element access
        // IDA: used by hkMeshVertexBufferUtil::getIndexedElementVectorArray etc.
        struct Buffer {
            hkVertexFormatDetail::Element m_element;
            const char* m_start;
            int m_stride;
        };

        // m_buffers - Array of buffer info for each element
        // IDA: accessed as lockedVertices->m_buffers[bufferIndex]
        Buffer m_buffers[32];  // Max 32 elements matching hkVertexFormat

        // findBufferIndex - Find buffer index by usage and subUsage
        // IDA: ?findBufferIndex@LockedVertices@hkMeshVertexBuffer@@QEBAHW4ComponentUsage@hkVertexFormat@@H@Z @ 0x1407ec310
        int findBufferIndex(hkVertexFormatDetail::ComponentUsage usage, int subUsage) const {
            for (int i = 0; i < m_numBuffers; i++) {
                if (m_buffers[i].m_element.m_usage.m_storage == static_cast<unsigned char>(usage) &&
                    m_buffers[i].m_element.m_subUsage == static_cast<unsigned char>(subUsage)) {
                    return i;
                }
            }
            return -1;
        }
    };

    // LockResult - Result of lock operation
    enum LockResult {
        LOCK_RESULT_SUCCESS = 0,
        LOCK_RESULT_FAILURE = 1,
    };

    // LockInput - Input for lock operation
    struct LockInput {
        int m_flags;
        int m_startVertex;
        int m_numVertices;
    };

    // PartialLockInput - Input for partial lock
    struct PartialLockInput {
        int m_flags;
    };

    // IDA: staticClass @ 0x1407e5e40
    static const hkClass* staticClass();

    // Virtual methods
    virtual int getNumVertices() { return 0; }
    virtual bool isSharable() { return true; }
    virtual void getVertexFormat(hkVertexFormat& format) {}
    virtual hkMeshVertexBuffer* clone() { return nullptr; }
    virtual LockResult lock(const LockInput& input, LockedVertices& lockedVertices) { return LOCK_RESULT_FAILURE; }
    virtual void unlock(const LockedVertices& lockedVertices) {}

    // IDA: getClassType - returns the hkClass for this type
    virtual const hkClass* getClassType() const { return staticClass(); }
};

// hkMeshShape - Base mesh shape class
class hkMeshShape : public hkReferencedObject {
public:
    // IDA: getNumSections virtual method
    virtual int getNumSections() const = 0;
    // IDA: lockSection @ 0x1407e6a80
    virtual void lockSection(int sectionIndex, unsigned char accessFlags, hkMeshSection* sectionOut) const = 0;
    // IDA: unlockSection @ 0x1407e6a60
    virtual void unlockSection(const hkMeshSection* section) const {}

    // IDA: getClassType - returns the hkClass for this type
    virtual const hkClass* getClassType() const = 0;

    // IDA: setName @ 0x1407e7090 - sets the shape name
    virtual void setName(const char* name) = 0;

    // IDA: getName @ 0x1407e70a0 - gets the shape name
    virtual const char* getName() const = 0;
};

// hkMeshBody - Base mesh body class
class hkMeshBody : public hkReferencedObject {
public:
    // IDA: staticClass @ 0x1407e5f20
    static const hkClass* staticClass();

    virtual hkMeshVertexBuffer* getVertexBuffer(int sectionIndex) = 0;
    virtual const hkMeshBoneIndexMapping* getIndexMappings() const = 0;
    virtual const hkMeshShape* getMeshShape() const = 0;
};

// hkMeshTexture - Mesh texture base class
class hkMeshTexture : public hkReferencedObject {
public:
    // Format enum
    enum Format {
        FORMAT_UNKNOWN = 0,
        FORMAT_R8G8B8 = 1,
        FORMAT_R8G8B8A8 = 2,
        FORMAT_BC1 = 3,
        FORMAT_BC2 = 4,
        FORMAT_BC3 = 5,
    };

    // FilterMode enum
    enum FilterMode {
        FILTER_MODE_NEAREST = 0,
        FILTER_MODE_LINEAR = 1,
    };

    // TextureUsageType enum
    enum TextureUsageType {
        TEXTURE_USAGE_TYPE_DEFAULT = 0,
        TEXTURE_USAGE_TYPE_DYNAMIC = 1,
        TEXTURE_USAGE_TYPE_IMMUTABLE = 2,
    };

    // IDA: staticClass @ 0x1407e5f30
    static const hkClass* staticClass();

    virtual void getData(unsigned char** data, int* size, Format* format) = 0;
    virtual FilterMode getFilterMode() const = 0;
    virtual void setFilterMode(FilterMode filterMode) = 0;
    virtual TextureUsageType getUsageHint() const = 0;
    virtual void setUsageHint(TextureUsageType hint) = 0;
    virtual void setData(unsigned char* data, int size, Format format) = 0;
};

// hkMeshSectionCinfo - Mesh section construction info
// IDA: size = 48 bytes (from hkMemoryMeshShape constructor usage)
struct hkMeshSectionCinfo {
    hkEnum<hkMeshSection::PrimitiveType, unsigned char> m_primitiveType;
    unsigned char m_padding[3];
    int m_numPrimitives;
    int m_vertexStartIndex;
    hkEnum<hkMeshSection::IndexType, unsigned char> m_indexType;
    unsigned char m_padding2[3];
    int m_transformIndex;
    hkMeshVertexBuffer* m_vertexBuffer;
    void* m_indices;
    hkMeshMaterial* m_material;

    // IDA: staticClass @ 0x1407e5d80
    static const hkClass* staticClass();
};

// hkMemoryMeshBody - Memory-backed mesh body implementation
// IDA: constructor @ 0x1407e60b0
class hkMemoryMeshBody : public hkMeshBody {
public:
    hkStringPtr m_name;
    hkRefPtr<hkMeshShape> m_shape;
    hkMatrix4 m_transform;
    hkRefPtr<class hkIndexedTransformSet> m_transformSet;
    hkArrayBase<hkMeshVertexBuffer*> m_vertexBuffers;

    // IDA: constructor @ 0x1407e60b0
    hkMemoryMeshBody(hkMeshSystem* meshSystem, const hkMeshShape* shape, const hkMatrix4* transform, hkIndexedTransformSetCinfo* transformSet);

    // IDA: serialization constructor @ 0x1407e5fa0
    hkMemoryMeshBody(hkFinishLoadedObjectFlag flag);

    // IDA: destructor @ 0x1407e5fd0
    ~hkMemoryMeshBody() override;

    // IDA: setName @ 0x1407e67d0
    void setName(const char* n) {
        m_name = n;
    }

    // IDA: getTransform @ 0x1407e67e0
    void getTransform(hkMatrix4* transform) const {
        *transform = m_transform;
    }

    // IDA: setTransform @ 0x1407e6800
    void setTransform(const hkMatrix4* matrix) {
        m_transform = *matrix;
    }

    // IDA: getVertexBuffer @ 0x1407e6820
    hkMeshVertexBuffer* getVertexBuffer(int sectionIndex) override {
        return m_vertexBuffers.m_data[sectionIndex];
    }

    // IDA: setIndexedTransforms @ 0x1407e6830
    void setIndexedTransforms(int startIndex, const hkMatrix4* matrices, int numMatrices) {
        if (m_transformSet.m_ptr) {
            m_transformSet.m_ptr->setMatrices(startIndex, matrices, numMatrices);
        }
    }

    // IDA: getIndexedTransforms @ 0x1407e6840
    void getIndexedTransforms(int startIndex, hkMatrix4* matrices, int numMatrices) {
        if (m_transformSet.m_ptr) {
            m_transformSet.m_ptr->getMatrices(startIndex, matrices, numMatrices);
        }
    }

    // IDA: getIndexedInverseTransforms @ 0x1407e6850
    void getIndexedInverseTransforms(int startIndex, hkMatrix4* matrices, int numMatrices) {
        if (m_transformSet.m_ptr) {
            m_transformSet.m_ptr->getInverseMatrices(startIndex, matrices, numMatrices);
        }
    }

    // IDA: getIndexTransformsOrder @ 0x1407e6860
    const short* getIndexTransformsOrder() const {
        return m_transformSet.m_ptr ? m_transformSet.m_ptr->m_matricesOrder.m_data : nullptr;
    }

    // IDA: getIndexMappings @ 0x1407e6880
    const hkMeshBoneIndexMapping* getIndexMappings() const override {
        return m_transformSet.m_ptr ? m_transformSet.m_ptr->m_indexMappings.m_data : nullptr;
    }

    // IDA: getMeshShape @ 0x1407e6890
    const hkMeshShape* getMeshShape() const override {
        return m_shape.m_ptr;
    }

    // IDA: getNumIndexedTransforms @ 0x1407e68a0
    int getNumIndexedTransforms() {
        return m_transformSet.m_ptr ? m_transformSet.m_ptr->m_matrices.m_size : 0;
    }

    // IDA: getNumIndexMappings @ 0x1407e68b0
    int getNumIndexMappings() const {
        return m_transformSet.m_ptr ? m_transformSet.m_ptr->m_indexMappings.m_size : 0;
    }
};

// hkMemoryMeshTexture - Memory-backed mesh texture implementation
class hkMemoryMeshTexture : public hkMeshTexture {
public:
    hkStringPtr m_filename;                        // IDA: m_filename used in setFilename/getFilename
    int m_textureCoordChannel;                      // IDA: m_textureCoordChannel used in getTextureCoordChannel
    hkArrayBase<unsigned char> m_data;
    hkEnum<Format, unsigned char> m_format;
    hkEnum<FilterMode, unsigned char> m_filterMode;
    hkEnum<TextureUsageType, unsigned char> m_usageHint;
    hkBool m_hasMipMaps;                           // IDA: m_hasMipMaps used in getHasMipMaps

    // IDA: constructor @ 0x1407e6a00
    hkMemoryMeshTexture() : m_textureCoordChannel(-1), m_format((Format)0), m_filterMode((FilterMode)2), m_usageHint((TextureUsageType)0), m_hasMipMaps(false) {}

    // IDA: setFilename @ 0x1407e6920
    void setFilename(const char* filename) {
        m_filename = filename;
    }

    // IDA: getTextureCoordChannel @ 0x1407e6930
    int getTextureCoordChannel() const {
        return m_textureCoordChannel;
    }

    // IDA: setTextureCoordChannel @ 0x1407e6940
    void setTextureCoordChannel(int channelIndex) {
        m_textureCoordChannel = channelIndex;
    }

    // IDA: getData @ 0x1407e6950
    void getData(unsigned char** data, int* size, Format* format) override {
        *data = m_data.m_data;
        *size = m_data.m_size;
        *format = m_format;
    }

    // IDA: getFilename @ 0x1407e6970
    const char* getFilename() const {
        return m_filename.cString();
    }

    // IDA: getHasMipMaps @ 0x1407e6980
    bool getHasMipMaps() const {
        return m_hasMipMaps.m_bool != 0;
    }

    // IDA: setHasMipMaps @ 0x1407e6990
    void setHasMipMaps(bool hasMipMaps) {
        m_hasMipMaps.m_bool = hasMipMaps;
    }

    // IDA: getFilterMode @ 0x1407e69a0
    FilterMode getFilterMode() const override {
        return m_filterMode;
    }

    // IDA: setFilterMode @ 0x1407e69b0
    void setFilterMode(FilterMode filterMode) override {
        m_filterMode = filterMode;
    }

    // IDA: getUsageHint @ 0x1407e69c0
    TextureUsageType getUsageHint() const override {
        return m_usageHint;
    }

    // IDA: setUsageHint @ 0x1407e69d0
    void setUsageHint(TextureUsageType hint) override {
        m_usageHint = hint;
    }

    // IDA: setData @ 0x1407e69e0
    void setData(unsigned char* data, int size, Format format) override {
        m_data.m_data = data;
        m_data.m_size = size;
        m_data.m_capacityAndFlags = size | 0x80000000;
        m_format = format;
    }
};

// hkMemoryMeshShape - Memory-backed mesh shape implementation
class hkMemoryMeshShape : public hkMeshShape {
public:
    hkStringPtr m_name;  // Shape name (must be before arrays for proper destruction order)
    hkArrayBase<hkMeshSectionCinfo> m_sections;
    hkArrayBase<unsigned short> m_indices16;
    hkArrayBase<unsigned int> m_indices32;

    // IDA: constructor @ 0x1407e6b30
    hkMemoryMeshShape(const hkMeshSectionCinfo* sections, int numSections);

    // IDA: constructor for deserialization @ 0x1407e6e50
    hkMemoryMeshShape(hkFinishLoadedObjectFlag f);

    // IDA: destructor @ 0x1407e6f40
    virtual ~hkMemoryMeshShape();

    // IDA: getNumSections
    int getNumSections() const override {
        return m_sections.m_size;
    }

    // IDA: lockSection @ 0x1407e6a80
    void lockSection(int sectionIndex, unsigned char accessFlags, hkMeshSection* sectionOut) const override;

    // IDA: unlockSection @ 0x1407e6a60
    void unlockSection(const hkMeshSection* section) const override {}

    // IDA: getClassType @ 0x1407e7080
    const hkClass* getClassType() const override;

    // IDA: setName @ 0x1407e7090
    void setName(const char* name) override {
        m_name = name;
    }

    // IDA: getName @ 0x1407e70a0
    const char* getName() const override {
        return m_name.cString();
    }
};

// hkSkinnedMeshShape - Skinned mesh shape base
class hkSkinnedMeshShape : public hkMeshShape {
public:
    // BoneSection - Bone section structure (16 bytes)
    // IDA: m_meshBuffer is a union/alias for the hkRefPtr internal storage
    struct BoneSection {
        union {
            hkRefPtr<hkMeshShape> m_meshShape;  // Regular hkRefPtr interface
            struct {
                hkMeshShape* m_meshBuffer;  // IDA shows this as m_meshBuffer.m_pntr
            };
        };
        short m_startBoneIdx;
        short m_numBones;

        // IDA: Default constructor @ 0x1407e71a0
        BoneSection() : m_meshShape(nullptr), m_startBoneIdx(0), m_numBones(0) {}

        // IDA: Constructor with hkFinishLoadedObjectFlag @ 0x1407e71c0
        BoneSection(hkFinishLoadedObjectFlag flag) : m_meshShape(nullptr), m_startBoneIdx(0), m_numBones(0) {}
    };

    // Part - Part structure (48 bytes based on addPart copy size)
    struct Part {
        int m_startVertex;
        int m_numVertices;
        int m_startIndex;
        int m_numIndices;
        int m_boneIndex;
        int m_meshSectionIndex;  // IDA shows this field name
        hkVector4 m_boundingSphere;  // 16 bytes

        Part() : m_startVertex(0), m_numVertices(0), m_startIndex(0), m_numIndices(0),
                 m_boneIndex(0), m_meshSectionIndex(0) {}
    };

    hkArrayBase<BoneSection> m_boneSections;

    // IDA: Default constructor @ 0x1407e7110
    hkSkinnedMeshShape() {
        m_memSizeAndFlags = static_cast<short>(-1);
        m_referenceCount = 1;
    }

    // IDA: Constructor with hkFinishLoadedObjectFlag @ 0x1407e7140
    hkSkinnedMeshShape(hkFinishLoadedObjectFlag flag) {
        // Just set vtable, no member initialization
    }

    // IDA: Destructor @ 0x1407e7160
    virtual ~hkSkinnedMeshShape() {}

    // IDA: getNumBoneSections
    virtual int getNumBoneSections() const { return m_boneSections.m_size; }

    // IDA: getBoneSection - virtual method to get bone section
    virtual void getBoneSection(int boneSectionIndex, BoneSection* boneSectionOut) const = 0;

    // IDA: getNumParts
    virtual int getNumParts() const = 0;

    // IDA: getPart - virtual method to get part
    virtual void getPart(int partIndex, Part* partOut) const = 0;

    // IDA: getClassType - returns the hkClass for this type
    virtual const hkClass* getClassType() const = 0;

    // IDA: setName - sets the shape name
    virtual void setName(const char* name) = 0;

    // IDA: getName - gets the shape name
    virtual const char* getName() const = 0;
};

// hkStorageSkinnedMeshShape - Storage skinned mesh shape
class hkStorageSkinnedMeshShape : public hkSkinnedMeshShape {
public:
    hkStringPtr m_name;  // Shape name
    hkArrayBase<Part> m_parts;

    // IDA: Default constructor @ 0x1407e7b90
    hkStorageSkinnedMeshShape();

    // IDA: Constructor with hkFinishLoadedObjectFlag @ 0x1407e7be0
    hkStorageSkinnedMeshShape(hkFinishLoadedObjectFlag flag);

    // IDA: Destructor @ 0x1407e7c70
    virtual ~hkStorageSkinnedMeshShape();

    // IDA: getClassType @ 0x1407e7180
    const hkClass* getClassType() const override;

    // IDA: setName @ 0x1407e7190
    void setName(const char* name) override { m_name = name; }

    // IDA: getName @ 0x1407e71f0
    const char* getName() const override { return m_name.cString(); }

    // IDA: getNumBoneSections @ 0x1407e71d0
    int getNumBoneSections() const override { return m_boneSections.m_size; }

    // IDA: getNumParts @ 0x1407e71e0
    int getNumParts() const override { return m_parts.m_size; }

    // IDA: addBoneSection @ 0x1407e73b0
    void addBoneSection(hkMeshShape* meshShape, short startBoneIdx, short numBones);

    // IDA: getBoneSection @ 0x1407e7200
    void getBoneSection(int boneSectionIndex, BoneSection* boneSectionOut) const override;

    // IDA: getPart @ 0x1407e7260
    void getPart(int partIndex, Part* partOut) const override;

    // IDA: addPart @ 0x1407e7470
    void addPart(const Part* p);

    // IDA: sortParts @ 0x1407e72b0
    void sortParts();
};

// hkSkinnedRefMeshShape - Skinned reference mesh shape
// References a hkSkinnedMeshShape with bone transforms
class hkSkinnedRefMeshShape : public hkMeshShape {
public:
    hkRefPtr<hkSkinnedMeshShape> m_skinnedMeshShape;  // Reference to skinned mesh shape
    hkArrayBase<short> m_bones;  // Bone indices
    hkArrayBase<hkQsTransform> m_localFromRootTransforms;  // Local transforms (32 bytes each)
    hkStringPtr m_name;  // Shape name

    // IDA: Constructor with skinned mesh @ 0x1407eb590
    hkSkinnedRefMeshShape(hkSkinnedMeshShape* fullSkin);

    // IDA: Constructor with hkFinishLoadedObjectFlag @ 0x1407eb610
    hkSkinnedRefMeshShape(hkFinishLoadedObjectFlag flag);

    // IDA: Destructor @ 0x1407eb640
    virtual ~hkSkinnedRefMeshShape();

    // IDA: getNumSections @ 0x1407eb460 - returns 0
    int getNumSections() const override { return 0; }

    // IDA: lockSection @ 0x1407eb470 - empty
    void lockSection(int sectionIndex, unsigned char accessFlags, hkMeshSection* sectionOut) const override {}

    // IDA: unlockSection @ 0x1407eb480 - empty
    void unlockSection(const hkMeshSection* section) const override {}

    // IDA: setName @ 0x1407eb490
    void setName(const char* name) override { m_name = name; }

    // IDA: getClassType @ 0x1407eb4a0
    const hkClass* getClassType() const override;

    // IDA: getName @ 0x1407eb4b0
    const char* getName() const override { return m_name.cString(); }

    // IDA: create static method @ 0x1407eb750
    static hkSkinnedRefMeshShape* create(const hkMeshShape* const* meshShapes, const hkQTransform* transforms, int numTransforms);
};

// hkMeshVertexBufferUtil - Mesh vertex buffer utility functions
namespace hkMeshVertexBufferUtil {
    // IDA: ?stridedZero@hkMeshVertexBufferUtil@@SAXPEAXHHH@Z @ 0x1407ed460
    // Zero out strided data in vertex buffer
    void stridedZero(void* dstIn, int dstStride, int elementSize, int numVertices);

    // IDA: ?stridedCopy@hkMeshVertexBufferUtil@@SAXPEBXHPEAXHHH@Z @ 0x1407ed5d0
    // Copy strided data between vertex buffers
    void stridedCopy(const void* srcIn, int srcStride, void* dstIn, int dstStride, int elementSize, int numVertices);

    // IDA: ?bufferIsSkinnable@hkMeshVertexBufferUtil@@SA?AVhkBool@@PEAVhkMeshVertexBuffer@@@Z @ 0x1407ed970
    // Check if buffer contains skinnable elements (position, normal, tangent, binormal)
    hkBool bufferIsSkinnable(hkMeshVertexBuffer* vertexBuffer);

    // IDA: ?bufferHasWeights@hkMeshVertexBufferUtil@@SA?AVhkBool@@PEAVhkMeshVertexBuffer@@@Z @ 0x1407eda20
    // Check if buffer has blend weights and matrix indices
    hkBool bufferHasWeights(hkMeshVertexBuffer* vertexBuffer);

    // IDA: hkMeshVertexBufferUtil_convertUint8ToInt16 @ 0x1407ed790
    // Convert uint8 values to int16
    void convertUint8ToInt16(const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer, const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer, int numVertices);

    // IDA: hkMeshVertexBufferUtil_convertUint8ToUint16 @ 0x1407ed800
    // Convert uint8 values to uint16
    void convertUint8ToUint16(const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer, const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer, int numVertices);

    // IDA: hkMeshVertexBufferUtil_convertInt16ToUint8 @ 0x1407ed870
    // Convert int16 values to uint8 (low byte)
    void convertInt16ToUint8(const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer, const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer, int numVertices);

    // IDA: ?getIndexedElementVectorArray@hkMeshVertexBufferUtil@@SA?AUhkResult@@AEBUBuffer@LockedVertices@hkMeshVertexBuffer@@PEBHPEAVhkVector4@@H@Z @ 0x1407edb00
    // Get indexed element vector array from buffer
    hkResult getIndexedElementVectorArray(const hkMeshVertexBuffer::LockedVertices::Buffer* buffer, const int* indices, hkVector4* out, int numVertices);

    // IDA: ?getElementVectorArray@hkMeshVertexBufferUtil@@SA?AUhkResult@@AEBUBuffer@LockedVertices@hkMeshVertexBuffer@@PEAVhkVector4@@H@Z @ 0x1407edd70
    // Get element vector array from single buffer
    hkResult getElementVectorArray(const hkMeshVertexBuffer::LockedVertices::Buffer* buffer, hkVector4* out, int numVertices);

    // IDA: ?setElementVectorArray@hkMeshVertexBufferUtil@@SA?AUhkResult@@AEBUBuffer@LockedVertices@hkMeshVertexBuffer@@PEBVhkVector4@@H@Z @ 0x1407edeb0
    // Set element vector array to single buffer
    hkResult setElementVectorArray(const hkMeshVertexBuffer::LockedVertices::Buffer* buffer, const hkVector4* src, int numVertices);

    // IDA: ?getElementIntArray@hkMeshVertexBufferUtil@@SA?AUhkResult@@AEBULockedVertices@hkMeshVertexBuffer@@HPEAH@Z @ 0x1407edfd0
    // Get element int array from locked vertices by buffer index
    hkResult getElementIntArray(const hkMeshVertexBuffer::LockedVertices* lockedVertices, int bufferIndex, int* dst);

    // IDA: ?setElementIntArray@hkMeshVertexBufferUtil@@SA?AUhkResult@@AEBULockedVertices@hkMeshVertexBuffer@@HPEBH@Z @ 0x1407ee2a0
    // Set element int array to locked vertices by buffer index
    hkResult setElementIntArray(const hkMeshVertexBuffer::LockedVertices* lockedVertices, int bufferIndex, const int* src);

    // IDA: hkMeshVertexBufferUtil_convertUint8DwordToUint8 @ 0x1407ee590
    // Convert uint8 dword to uint8 values
    void convertUint8DwordToUint8(const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer, const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer, unsigned int numVertices);

    // IDA: hkMeshVertexBufferUtil_convertUint8DwordToArgb32 @ 0x1407ee820
    // Convert uint8 dword to ARGB32
    void convertUint8DwordToArgb32(const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer, const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer, int numVertices);

    // IDA: hkMeshVertexBufferUtil_convertArgb32ToToFloat32 @ 0x1407ee890
    // Convert ARGB32 to Float32 (normalize to 0-1)
    void convertArgb32ToFloat32(const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer, const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer, int numVertices);

    // IDA: hkMeshVertexBufferUtil_convertUint8DwordToFloat32 @ 0x1407eeb80
    // Convert uint8 dword to Float32
    void convertUint8DwordToFloat32(const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer, const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer, int numVertices);

    // IDA: hkMeshVertexBufferUtil_convertUint8ToUint8Dword @ 0x1407eed90
    // Convert uint8 values to uint8 dword
    void convertUint8ToUint8Dword(const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer, const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer, unsigned int numVertices);

    // IDA: hkMeshVertexBufferUtil_convertUint8DwordToInt16 @ 0x1407ee780
    // Convert uint8 dword to int16
    void convertUint8DwordToInt16(const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer, const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer, unsigned int numVertices);

    // IDA: hkMeshVertexBufferUtil_convertUint8ToFloat32 @ 0x1407eefc0
    // Convert uint8 values to Float32 (normalize to 0-1)
    void convertUint8ToFloat32(const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer, const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer, unsigned int numVertices);

    // IDA: hkMeshVertexBufferUtil_convertFloat32ToUint8 @ 0x1407ef2e0
    // Convert Float32 to uint8 (denormalize from 0-1)
    void convertFloat32ToUint8(const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer, const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer, unsigned int numVertices);

    // IDA: ?copy@hkMeshVertexBufferUtil@@SAXAEBUBuffer@LockedVertices@hkMeshVertexBuffer@@0H@Z @ 0x1407ee540
    // Copy buffer data using strided copy
    void copy(const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer, const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer, int numVertices);

    // IDA: hkMeshVertexBufferUtil_convertFloat323FloatsToFloat324Floats @ 0x1407ef5f0
    // Convert 3-float vectors to 4-float vectors (w = 0)
    void convertFloat323FloatsToFloat324Floats(const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer, const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer, int numVertices);

    // IDA: hkMeshVertexBufferUtil_convertFloat32ToArgb32 @ 0x1407f0220
    // Convert Float32 color to ARGB32
    void convertFloat32ToArgb32(const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer, const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer, unsigned int numVertices);

    // IDA: ?isBufferDataEqual@hkMeshVertexBufferUtil@@SA?AVhkBool@@AEBUBuffer@LockedVertices@hkMeshVertexBuffer@@0M@Z @ 0x1407efb30
    // Compare buffer data with threshold tolerance
    hkBool isBufferDataEqual(const hkMeshVertexBuffer::LockedVertices::Buffer* bufferA, const hkMeshVertexBuffer::LockedVertices::Buffer* bufferB, float threshold);

    // IDA: ?mergeVertexFormat@hkMeshVertexBufferUtil@@SAXAEAUhkVertexFormat@@AEBU2@@Z @ 0x1407efd80
    // Merge two vertex formats
    void mergeVertexFormat(hkVertexFormat& dstFmt, const hkVertexFormat& srcFmt);

    // IDA: ?partitionVertexFormat@hkMeshVertexBufferUtil@@SAXAEBUhkVertexFormat@@AEAU2@1@Z @ 0x1407ef740
    // Partition vertex format into shared and instance formats
    void partitionVertexFormat(const hkVertexFormat& format, hkVertexFormat& sharedFormat, hkVertexFormat& instanceFormat);

    // IDA: hkMeshVertexBufferUtil_interpolateCopy @ 0x1407ef7c0
    // Simple interpolation copy - copies src or dst based on interp value
    void interpolateCopy(const hkVertexFormatDetail::Element* element, const void* srcA, const void* srcB, float interp, void* dst);

    // IDA: hkMeshVertexBufferUtil_interpolate @ 0x1407ef850
    // Complex interpolation for ARGB32, FLOAT32, FLOAT324 types (by data type)
    void interpolateByDataType(const hkVertexFormatDetail::Element* element, const void* srcA, const void* srcB, float interp, void* dst);

    // IDA: ?computeMostFittingVertexFormat@hkMeshVertexBufferUtil@@SAXAEAUhkVertexFormat@@PEBU2@H@Z @ 0x1407efe50
    // Compute most fitting vertex format by merging all source formats
    void computeMostFittingVertexFormat(hkVertexFormat& dstFormat, const hkVertexFormat* srcFormats, unsigned int numSourceFormats);

    // IDA: ?isBufferNormalDataEqual@hkMeshVertexBufferUtil@@SA?AVhkBool@@AEBUBuffer@LockedVertices@hkMeshVertexBuffer@@0M@Z @ 0x1407f0890
    // Check if normal data is equal within threshold
    hkBool isBufferNormalDataEqual(const hkMeshVertexBuffer::LockedVertices::Buffer* bufferA, const hkMeshVertexBuffer::LockedVertices::Buffer* bufferB, float threshold);

    // IDA: ?isBufferDataEqual@hkMeshVertexBufferUtil@@SA?AVhkBool@@PEBUBuffer@LockedVertices@hkMeshVertexBuffer@@0HAEBUThresholds@1@@Z @ 0x1407f0950
    // Check if buffer data is equal with thresholds per component type
    struct Thresholds {
        float m_positionThreshold;
        float m_normalThreshold;
        float m_colorThreshold;
        float m_texCoordThreshold;
        float m_otherThreshold;
    };
    hkBool isBufferDataEqual(const hkMeshVertexBuffer::LockedVertices::Buffer* buffersA, const hkMeshVertexBuffer::LockedVertices::Buffer* buffersB, int numBuffers, const Thresholds& thresholds);

    // IDA: ?transform@hkMeshVertexBufferUtil@@SAXAEBUBuffer@LockedVertices@hkMeshVertexBuffer@@AEBVhkMatrix4@@HH@Z @ 0x1407f0c60
    // Transform buffer data with matrix (for position, normal, tangent, binormal)
    void transform(const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer, const hkMatrix4* transform, int transformFlags, unsigned int numVertices);

    // IDA: ?transform@hkMeshVertexBufferUtil@@SA?AUhkResult@@PEAVhkMeshVertexBuffer@@AEBVhkMatrix4@@H@Z @ 0x1407f0f00
    // Transform entire vertex buffer with matrix
    hkResult transform(hkMeshVertexBuffer* buffer, const hkMatrix4* transform, int transformFlags);

    // IDA: ?isContiguous@hkMeshVertexBufferUtil@@SA?AVhkBool@@AEBULockedVertices@hkMeshVertexBuffer@@PEAPEAXAEAH@Z @ 0x1407f0ff0
    // Check if locked vertices are contiguous in memory
    hkBool isContiguous(const hkMeshVertexBuffer::LockedVertices* lockedVertices, void** startOut, int* dataSize);

    // IDA: hkMeshVertexBufferUtil_interpolateNormalize @ 0x1407f1210
    // Interpolate with normalization (for normals/tangents)
    void interpolateNormalize(const hkVertexFormatDetail::Element* element, const void* srcA, const void* srcB, float interp, void* dst);

    // IDA: ?interpolate@hkMeshVertexBufferUtil@@SAXAEBUElement@hkVertexFormat@@PEBX1MPEAX@Z @ 0x1407f1660
    // Interpolate element based on usage (dispatches to appropriate interpolation)
    void interpolate(const hkVertexFormatDetail::Element* element, const void* srcA, const void* srcB, float interp, void* dst);

    // IDA: ?copy@hkMeshVertexBufferUtil@@SAXAEBULockedVertices@hkMeshVertexBuffer@@0@Z @ 0x1407eff30
    // Copy all buffers in locked vertices
    void copy(const hkMeshVertexBuffer::LockedVertices* srcVertices, const hkMeshVertexBuffer::LockedVertices* dstVertices);

    // IDA: ?getElementVectorArray@hkMeshVertexBufferUtil@@SA?AUhkResult@@PEAVhkMeshVertexBuffer@@W4ComponentUsage@hkVertexFormat@@HAEAV?$hkArray@VhkVector4@@UhkContainerHeapAllocator@@@@@Z @ 0x1407f0b00
    // Get element vectors from vertex buffer
    hkResult getElementVectorArray(hkMeshVertexBuffer* vertexBuffer, hkVertexFormatDetail::ComponentUsage usage, int subUsage, hkArray<hkVector4, hkContainerHeapAllocator>* vectorsOut);

    // Buffer ordering function for sorting
    // IDA: ?hkMeshVertexBufferUtil_orderBuffers@@YA_NAEBUBuffer@LockedVertices@hkMeshVertexBuffer@@0@Z @ 0x1407f16f0
    inline bool orderBuffers(const hkMeshVertexBuffer::LockedVertices::Buffer& a, const hkMeshVertexBuffer::LockedVertices::Buffer& b) {
        return a.m_start < b.m_start;
    }

    // Utility functions used by hkMultipleVertexBuffer (overloads using LockedVertices directly)
    void getElementVectorArray(hkResult* result, const hkMeshVertexBuffer::LockedVertices* lockedVertices, int elementIndex, hkVector4* data);
    void setElementVectorArray(hkResult* result, const hkMeshVertexBuffer::LockedVertices* lockedVertices, int elementIndex, const hkVector4* data);
    void getElementIntArray(hkResult* result, const hkMeshVertexBuffer::LockedVertices* lockedVertices, int elementIndex, int* data);
    void setElementIntArray(hkResult* result, const hkMeshVertexBuffer::LockedVertices* lockedVertices, int elementIndex, const int* data);
}

// hkVertexFormat - Vertex format description
// Element structure is 8 bytes based on IDA
class hkVertexFormat {
public:
    // Import enums from namespace for backward compatibility
    typedef hkVertexFormatDetail::ComponentUsage ComponentUsage;
    typedef hkVertexFormatDetail::ComponentType ComponentType;
    typedef hkVertexFormatDetail::Element Element;

    // Re-export enum values for convenience
    static constexpr ComponentUsage USAGE_POSITION = hkVertexFormatDetail::USAGE_POSITION;
    static constexpr ComponentUsage USAGE_NORMAL = hkVertexFormatDetail::USAGE_NORMAL;
    static constexpr ComponentUsage USAGE_COLOR = hkVertexFormatDetail::USAGE_COLOR;
    static constexpr ComponentUsage USAGE_TANGENT = hkVertexFormatDetail::USAGE_TANGENT;
    static constexpr ComponentUsage USAGE_BINORMAL = hkVertexFormatDetail::USAGE_BINORMAL;
    static constexpr ComponentUsage USAGE_TEXTURE_COORD = hkVertexFormatDetail::USAGE_TEXTURE_COORD;
    static constexpr ComponentUsage USAGE_BLEND_INDICES = hkVertexFormatDetail::USAGE_BLEND_INDICES;
    static constexpr ComponentUsage USAGE_BLEND_WEIGHTS = hkVertexFormatDetail::USAGE_BLEND_WEIGHTS;
    static constexpr ComponentUsage USAGE_USER = hkVertexFormatDetail::USAGE_USER;
    static constexpr ComponentUsage USAGE_BLEND_MATRIX_INDEX = hkVertexFormatDetail::USAGE_BLEND_MATRIX_INDEX;
    static constexpr ComponentUsage USAGE_BLEND_WEIGHTS_LAST_IMPLIED = hkVertexFormatDetail::USAGE_BLEND_WEIGHTS_LAST_IMPLIED;

    static constexpr ComponentType TYPE_NONE = hkVertexFormatDetail::TYPE_NONE;
    static constexpr ComponentType TYPE_FLOAT = hkVertexFormatDetail::TYPE_FLOAT;
    static constexpr ComponentType TYPE_INT8 = hkVertexFormatDetail::TYPE_INT8;
    static constexpr ComponentType TYPE_UINT8 = hkVertexFormatDetail::TYPE_UINT8;
    static constexpr ComponentType TYPE_INT16 = hkVertexFormatDetail::TYPE_INT16;
    static constexpr ComponentType TYPE_UINT16 = hkVertexFormatDetail::TYPE_UINT16;
    static constexpr ComponentType TYPE_INT32 = hkVertexFormatDetail::TYPE_INT32;
    static constexpr ComponentType TYPE_UINT32 = hkVertexFormatDetail::TYPE_UINT32;
    static constexpr ComponentType TYPE_ARGB32 = hkVertexFormatDetail::TYPE_ARGB32;
    static constexpr ComponentType TYPE_FLOAT32 = hkVertexFormatDetail::TYPE_FLOAT32;

    int m_numElements;
    Element m_elements[32];  // Max 32 elements

    // IDA: ??0hkVertexFormat@@QEAA@XZ @ 0x1407ec040
    hkVertexFormat() : m_numElements(0) {}

    // IDA: ??0hkVertexFormat@@QEAA@AEBU0@@Z @ 0x1407ec060
    hkVertexFormat(const hkVertexFormat& rhs) {
        set(rhs);
    }

    // IDA: ?set@hkVertexFormat@@QEAAXAEBU1@@Z @ 0x1407ebfa0
    void set(const hkVertexFormat& rhs) {
        m_numElements = rhs.m_numElements;
        for (int i = 0; i < m_numElements; i++) {
            m_elements[i] = rhs.m_elements[i];
        }
    }

    // IDA: ??4hkVertexFormat@@QEAAXAEBU0@@Z @ 0x1407ec080
    hkVertexFormat& operator=(const hkVertexFormat& rhs) {
        set(rhs);
        return *this;
    }

    // IDA: ??8hkVertexFormat@@QEBA_NAEBU0@@Z @ 0x1407ec090
    bool operator==(const hkVertexFormat& rhs) const {
        if (m_numElements != rhs.m_numElements) return false;
        for (int i = 0; i < m_numElements; i++) {
            const Element& a = m_elements[i];
            const Element& b = rhs.m_elements[i];
            if (a.m_dataType != b.m_dataType || a.m_numValues != b.m_numValues ||
                a.m_flags != b.m_flags || a.m_usage != b.m_usage ||
                a.m_subUsage != b.m_subUsage) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const hkVertexFormat& rhs) const {
        return !(*this == rhs);
    }

    // IDA: ?findNextSubUsage@hkVertexFormat@@QEBAHW4ComponentUsage@1@@Z @ 0x1407ec100
    int findNextSubUsage(ComponentUsage usage) const {
        int maxSubUsage = -1;
        for (int i = 0; i < m_numElements; i++) {
            if (m_elements[i].m_usage == usage) {
                if (m_elements[i].m_subUsage > maxSubUsage) {
                    maxSubUsage = m_elements[i].m_subUsage;
                }
            }
        }
        return maxSubUsage + 1;
    }

    // IDA: ?findElementIndex@hkVertexFormat@@QEBAHW4ComponentUsage@1@H@Z @ 0x1407ec140
    int findElementIndex(ComponentUsage usage, int subUsage) const {
        for (int i = 0; i < m_numElements; i++) {
            if (m_elements[i].m_usage == usage && m_elements[i].m_subUsage == subUsage) {
                return i;
            }
        }
        return -1;
    }

    // IDA: ?addElement@hkVertexFormat@@QEAAXW4ComponentUsage@1@W4ComponentType@1@HH@Z @ 0x1407ec210
    void addElement(ComponentUsage usage, ComponentType type, int numValues, int flags) {
        if (m_numElements < 32) {
            int subUsage = findNextSubUsage(usage);
            Element& elem = m_elements[m_numElements];
            elem.m_dataType = type;
            elem.m_numValues = numValues;
            elem.m_flags = flags;
            elem.m_usage = usage;
            elem.m_subUsage = subUsage;
            m_numElements++;
        }
    }

    // Add element directly from another Element structure
    void addElement(const Element& elem) {
        if (m_numElements < 32) {
            m_elements[m_numElements] = elem;
            m_numElements++;
        }
    }

    // IDA: ?isCanonicalOrder@hkVertexFormat@@QEBA_NXZ @ 0x1407ec180
    bool isCanonicalOrder() const;

    // IDA: ?makeCanonicalOrder@hkVertexFormat@@QEAAXXZ @ 0x1407ec350
    void makeCanonicalOrder();
};

static_assert(sizeof(hkVertexFormat::Element) == 8, "hkVertexFormat::Element size mismatch - expected 8 bytes");

// hkMemoryMeshVertexBuffer - Memory-backed mesh vertex buffer
class hkMemoryMeshVertexBuffer : public hkMeshVertexBuffer {
public:
    // Member variables
    hkVertexFormat m_format;
    hkArrayBase<unsigned char> m_memory;  // Vertex data buffer
    hkBool m_locked;
    int m_numVertices;
    hkBool m_isSharable;
    int m_vertexStride;
    hkBool m_isBigEndian;
    int m_elementOffsets[32];  // Pre-computed offsets for each element in format

    // IDA: Default constructor @ 0x1407eb070
    hkMemoryMeshVertexBuffer();

    // IDA: Constructor with format and numVertices @ 0x1407eb270
    hkMemoryMeshVertexBuffer(const hkVertexFormat& format, int numVertices);

    // IDA: Constructor with hkFinishLoadedObjectFlag @ 0x1407eb0f0
    hkMemoryMeshVertexBuffer(hkFinishLoadedObjectFlag flag);

    // IDA: Destructor @ 0x1407eb1f0
    virtual ~hkMemoryMeshVertexBuffer();

    // IDA: staticClass @ 0x1407e5db0
    static const hkClass* staticClass();

    // IDA: getNumVertices @ 0x1407eb3e0
    int getNumVertices() override { return m_numVertices; }

    // IDA: isSharable @ 0x1407eb3f0
    bool isSharable() override { return m_isSharable.m_bool != 0; }

    // IDA: clone @ 0x1407eb310
    hkMeshVertexBuffer* clone() override;

    // IDA: lock @ 0x1407eb000
    LockResult lock(const LockInput& input, LockedVertices& lockedVertices) override;

    // IDA: unlock @ 0x1407eaa70
    void unlock(const LockedVertices& lockedVertices) override;

    // IDA: setVertexFormat @ 0x1407eafa0
    void setVertexFormat(const hkVertexFormat& format);

    // IDA: setNumVerticesAndZero @ 0x1407eb130
    void setNumVerticesAndZero(int numVertices);

    // IDA: handleEndian @ 0x1407eab00 - Handle endianness conversion for loaded data
    void handleEndian();

    // IDA: useExternalBuffer @ 0x1407eb040
    void useExternalBuffer(void* buffer, int size, int numVertices);

    // IDA: getElementVectorArray @ 0x1407eaa80
    void getElementVectorArray(const LockedVertices* lockedVertices, int elementIndex, hkVector4* data);

    // IDA: setElementVectorArray @ 0x1407eaaa0
    void setElementVectorArray(const LockedVertices* lockedVertices, int elementIndex, const hkVector4* data);

    // IDA: getElementIntArray @ 0x1407eaac0
    void getElementIntArray(const LockedVertices* lockedVertices, int elementIndex, int* data);

    // IDA: setElementIntArray @ 0x1407eaae0
    void setElementIntArray(const LockedVertices* lockedVertices, int elementIndex, const int* data);
};

// hkMultipleVertexBuffer - Multiple vertex buffer implementation
class hkMultipleVertexBuffer : public hkMeshVertexBuffer {
public:
    // VertexBufferInfo - Information about each vertex buffer (24 bytes)
    struct VertexBufferInfo {
        hkRefPtr<hkMeshVertexBuffer> m_vertexBuffer;  // 8 bytes
        int m_startVertex;
        int m_numVertices;
        int m_pad;  // Padding to 24 bytes

        VertexBufferInfo() : m_startVertex(0), m_numVertices(0), m_pad(0) {}
    };

    // ElementInfo - Information about each element (2 bytes)
    struct ElementInfo {
        unsigned char m_elementType;
        unsigned char m_elementIndex;

        ElementInfo() : m_elementType(0), m_elementIndex(0) {}
    };

    // LockedElement - Information about locked elements (7 bytes based on array size)
    struct LockedElement {
        int m_bufferIndex;
        unsigned char m_elementType;
        unsigned char m_elementIndex;
        unsigned char m_pad0;
        unsigned char m_pad1;

        LockedElement() : m_bufferIndex(0), m_elementType(0), m_elementIndex(0), m_pad0(0), m_pad1(0) {}
    };

    // Member variables
    hkVertexFormat m_vertexFormat;
    int m_numVertices;
    hkBool m_isLocked;
    int m_updateCount;
    hkBool m_constructionComplete;
    hkArrayBase<VertexBufferInfo> m_vertexBufferInfos;  // 24 bytes each
    hkArrayBase<ElementInfo> m_elementInfos;  // 2 bytes each
    hkRefPtr<hkMemoryMeshVertexBuffer> m_lockedBuffer;
    hkArrayBase<LockedElement> m_lockedElements;  // 7 bytes each

    // IDA: Constructor with format and numVertices @ 0x1407e88f0
    hkMultipleVertexBuffer(const hkVertexFormat& format, int numVertices);

    // IDA: Constructor with hkFinishLoadedObjectFlag @ 0x1407e8a40
    hkMultipleVertexBuffer(hkFinishLoadedObjectFlag flag);

    // IDA: Copy constructor @ 0x1407e8a60
    hkMultipleVertexBuffer(const hkMultipleVertexBuffer& other);

    // IDA: Destructor @ 0x1407e90c0
    virtual ~hkMultipleVertexBuffer();

    // IDA: getClassType @ 0x1407e90b0
    const hkClass* getClassType() const override;

    // IDA: getNumVertices @ 0x1407e90a0
    int getNumVertices() override { return m_numVertices; }

    // IDA: isSharable @ 0x1407e9080
    bool isSharable() override;

    // IDA: getVertexFormat @ 0x1407e9090
    void getVertexFormat(hkVertexFormat& format) override;

    // IDA: clone @ 0x1407e8e20
    hkMeshVertexBuffer* clone() override;

    // IDA: getElementVectorArray @ 0x1407e7dd0
    void getElementVectorArray(const LockedVertices* lockedVertices, int elementIndex, hkVector4* data) {
        hkResult result;
        hkMeshVertexBufferUtil::getElementVectorArray(&result, lockedVertices, elementIndex, data);
    }

    // IDA: setElementVectorArray @ 0x1407e7df0
    void setElementVectorArray(const LockedVertices* lockedVertices, int elementIndex, const hkVector4* data) {
        hkResult result;
        hkMeshVertexBufferUtil::setElementVectorArray(&result, lockedVertices, elementIndex, data);
    }

    // IDA: getElementIntArray @ 0x1407e7e10
    void getElementIntArray(const LockedVertices* lockedVertices, int elementIndex, int* data) {
        hkResult result;
        hkMeshVertexBufferUtil::getElementIntArray(&result, lockedVertices, elementIndex, data);
    }

    // IDA: setElementIntArray @ 0x1407e7e30
    void setElementIntArray(const LockedVertices* lockedVertices, int elementIndex, const int* data) {
        hkResult result;
        hkMeshVertexBufferUtil::setElementIntArray(&result, lockedVertices, elementIndex, data);
    }
};

// ============================================================================
// hkMergeMeshPrimitvesCalculator - Calculator for merging mesh primitives
// ============================================================================

struct hkMergeMeshPrimitvesCalculator {
    int m_numTotalPrimitives;
    hkMeshSection::PrimitiveType m_mergedPrimitiveType;
    hkMeshSection::IndexType m_mergedIndexType;

    hkMergeMeshPrimitvesCalculator()
        : m_numTotalPrimitives(0)
        , m_mergedPrimitiveType(hkMeshSection::PRIMITIVE_TYPE_TRIANGLE_LIST)
        , m_mergedIndexType(hkMeshSection::INDEX_TYPE_NONE) {}

    // IDA: ?add@hkMergeMeshPrimitvesCalculator@@QEAAXHW4PrimitiveType@hkMeshSection@@W4MeshSectionIndexType@3@@Z @ 0x1407ec950
    void add(int numPrimitives, hkMeshSection::PrimitiveType primitiveType, hkMeshSection::IndexType indexType);
};

// hkMeshPrimitiveUtil - Mesh primitive utility functions
namespace hkMeshPrimitiveUtil {
    // PrimitiveStyle - Style of primitives for rendering
    enum PrimitiveStyle {
        PRIMITIVE_STYLE_TRIANGLES = 0,
        PRIMITIVE_STYLE_TRIANGLE_STRIP = 1,
        PRIMITIVE_STYLE_TRIANGLE_FAN = 2,
        PRIMITIVE_STYLE_LINES = 3,
    };

    // IDA: ?getPrimitiveStyle@hkMeshPrimitiveUtil@@SA?AW4PrimitiveStyle@1@W4PrimitiveType@hkMeshSection@@@Z @ 0x1407ec530
    inline PrimitiveStyle getPrimitiveStyle(hkMeshSection::PrimitiveType type) {
        if (type == hkMeshSection::PRIMITIVE_TYPE_TRIANGLE_LIST)
            return PRIMITIVE_STYLE_TRIANGLES;
        if (type == hkMeshSection::PRIMITIVE_TYPE_TRIANGLE_STRIP)
            return PRIMITIVE_STYLE_TRIANGLE_STRIP;
        if (type == hkMeshSection::PRIMITIVE_TYPE_TRIANGLE_FAN)
            return PRIMITIVE_STYLE_TRIANGLE_FAN;
        if (type == hkMeshSection::PRIMITIVE_TYPE_LINE_LIST ||
            type == hkMeshSection::PRIMITIVE_TYPE_LINE_STRIP)
            return PRIMITIVE_STYLE_LINES;
        return PRIMITIVE_STYLE_TRIANGLES;
    }

    // IDA: ?calculateNumIndices@hkMeshPrimitiveUtil@@SAHW4PrimitiveType@hkMeshSection@@H@Z @ 0x1407ec560
    // Calculates number of indices based on primitive type and primitive count
    inline int calculateNumIndices(hkMeshSection::PrimitiveType type, unsigned int numPrims) {
        switch (type) {
            case hkMeshSection::PRIMITIVE_TYPE_TRIANGLE_STRIP:
                return numPrims;
            case hkMeshSection::PRIMITIVE_TYPE_TRIANGLE_FAN:
                return numPrims * 2;
            case hkMeshSection::PRIMITIVE_TYPE_TRIANGLE_LIST:
                return numPrims * 3;
            case hkMeshSection::PRIMITIVE_TYPE_LINE_STRIP:
                return numPrims + 2;
            default:
                return 0;
        }
    }

    // IDA: ?calculateNumPrimitives@hkMeshPrimitiveUtil@@SAHW4PrimitiveType@hkMeshSection@@H@Z @ 0x1407ec590
    // Calculates number of primitives based on primitive type and index count
    inline int calculateNumPrimitives(hkMeshSection::PrimitiveType type, int numIndices) {
        switch (type) {
            case hkMeshSection::PRIMITIVE_TYPE_TRIANGLE_STRIP:
                return numIndices;
            case hkMeshSection::PRIMITIVE_TYPE_TRIANGLE_FAN:
                return numIndices / 2;
            case hkMeshSection::PRIMITIVE_TYPE_TRIANGLE_LIST:
                return numIndices / 3;
            case hkMeshSection::PRIMITIVE_TYPE_LINE_STRIP:
                return numIndices - 2;
            default:
                return 0;
        }
    }

    // IDA: ?copyPrimitives@hkMeshPrimitiveUtil@@SAXPEBXW4PrimitiveType@hkMeshSection@@W4MeshSectionIndexType@3@HPEAX12H@Z @ 0x1407ec5f0
    // Copies primitives from source to destination, handling format conversion
    void copyPrimitives(
        const void* srcIndexPtr,
        hkMeshSection::PrimitiveType srcPrimitiveType,
        hkMeshSection::IndexType srcIndexType,
        int numPrimitives,
        void* dstIndexPtr,
        hkMeshSection::PrimitiveType dstPrimitiveType,
        hkMeshSection::IndexType dstIndexType,
        int baseIndexOffset);

    // IDA: ?appendTriangleIndices@hkMeshPrimitiveUtil@@SAXW4PrimitiveType@hkMeshSection@@HHAEAV?$hkArrayBase@G@@AEAVhkMemoryAllocator@@@Z @ 0x1407ec9c0
    // Appends triangle indices (16-bit) from primitive type
    void appendTriangleIndices(
        hkMeshSection::PrimitiveType primType,
        int numVertices,
        short indexBase,
        hkArrayBase<unsigned short>* indicesOut,
        hkMemoryAllocator* allocator);

    // IDA: ?appendTriangleIndices@hkMeshPrimitiveUtil@@SAXW4PrimitiveType@hkMeshSection@@HHAEAV?$hkArray@IUhkContainerHeapAllocator@@@@@Z @ 0x1407ecb00
    // Appends triangle indices (32-bit) from primitive type
    void appendTriangleIndices(
        hkMeshSection::PrimitiveType primType,
        int numVertices,
        int indexBase,
        hkArray<unsigned int, hkContainerHeapAllocator>* indicesOut);

    // IDA: ?appendTriangleIndices16@hkMeshPrimitiveUtil@@SAXW4PrimitiveType@hkMeshSection@@PEBGHHAEAV?$hkArray@GUhkContainerHeapAllocator@@@@@Z @ 0x1407ecc50
    // Appends triangle indices (16-bit) from 16-bit source indices
    void appendTriangleIndices16(
        hkMeshSection::PrimitiveType primType,
        const unsigned short* srcIndices,
        int numIndices,
        int indexBase,
        hkArray<unsigned short, hkContainerHeapAllocator>* indicesOut);

    // IDA: ?appendTriangleIndices32@hkMeshPrimitiveUtil@@SAXW4PrimitiveType@hkMeshSection@@PEBIHHAEAV?$hkArray@IUhkContainerHeapAllocator@@@@@Z @ 0x1407ece10
    // Appends triangle indices (32-bit) from 32-bit source indices
    void appendTriangleIndices32(
        hkMeshSection::PrimitiveType primType,
        const unsigned int* srcIndices,
        int numIndices,
        int indexBase,
        hkArray<unsigned int, hkContainerHeapAllocator>* indicesOut);

    // IDA: ?appendTriangleIndices@hkMeshPrimitiveUtil@@SAXAEBUhkMeshSection@@AEAV?$hkArray@GUhkContainerHeapAllocator@@@@@Z @ 0x1407ecfa0
    // Appends triangle indices from mesh section (16-bit output)
    void appendTriangleIndices(
        const hkMeshSection* section,
        hkArray<unsigned short, hkContainerHeapAllocator>* indicesOut);

    // IDA: ?appendTriangleIndices@hkMeshPrimitiveUtil@@SAXAEBUhkMeshSection@@AEAV?$hkArray@IUhkContainerHeapAllocator@@@@@Z @ 0x1407ed1e0
    // Appends triangle indices from mesh section (32-bit output)
    void appendTriangleIndices(
        const hkMeshSection* section,
        hkArray<unsigned int, hkContainerHeapAllocator>* indicesOut);

    // IDA: ?appendTriangleIndices@hkMeshPrimitiveUtil@@SAXPEBVhkMeshShape@@HAEAV?$hkArray@GUhkContainerHeapAllocator@@@@@Z @ 0x1407ed410
    // Appends triangle indices from mesh shape (locks section, gets indices, unlocks)
    void appendTriangleIndices(
        const hkMeshShape* shape,
        int sectionIndex,
        hkArray<unsigned short, hkContainerHeapAllocator>* indicesOut);

    // Additional overload for line strip (LINE_STRIP case in IDA)
    // IDA: handles primType - 3 == 0 (LINE_LIST) case
    inline void appendTriangleIndices_LineList(
        hkArrayBase<unsigned short>* indicesOut,
        hkMemoryAllocator* allocator,
        int numVertices,
        short indexBase) {
        int m_size = indicesOut->m_size;
        int neededCapacity = m_size + numVertices;
        int currentCapacity = indicesOut->m_capacityAndFlags & 0x3FFFFFFF;

        if (currentCapacity < neededCapacity) {
            int newCapacity = currentCapacity * 2;
            if (neededCapacity < newCapacity)
                neededCapacity = newCapacity;
            hkResult res;
            hkArrayUtil::_reserve(&res, *allocator, reinterpret_cast<void**>(indicesOut), neededCapacity, 2);
        }

        unsigned short* data = indicesOut->m_data;
        indicesOut->m_size += numVertices;

        for (int i = 0; i < numVertices; i++) {
            data[m_size + i] = indexBase + i;
        }
    }
}

// ============================================================================
// hkGeometry - Havok Geometry Structure
// ============================================================================

// Forward declaration
struct hkGeometry;
struct hkMatrix4;

// hkGeometry - Geometry structure containing vertices and triangles
// Used for physics mesh representation
struct hkGeometry {
    // Triangle - Triangle index structure (16 bytes)
    struct Triangle {
        int m_a;          // First vertex index
        int m_b;          // Second vertex index
        int m_c;          // Third vertex index
        int m_material;   // Material index

        Triangle() : m_a(0), m_b(0), m_c(0), m_material(0) {}
        Triangle(int a, int b, int c, int material) : m_a(a), m_b(b), m_c(c), m_material(material) {}
    };

    // Member variables
    hkArrayBase<hkVector4> m_vertices;      // Array of vertex positions
    hkArrayBase<Triangle> m_triangles;      // Array of triangles

    // IDA: ??0hkGeometry@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x14079e420
    // Constructor with hkFinishLoadedObjectFlag - does nothing (serialization)
    hkGeometry(hkFinishLoadedObjectFlag f) {}

    // IDA: ??0hkGeometry@@QEAA@AEBU0@@Z @ 0x14079e2a0
    // Copy constructor
    hkGeometry(const hkGeometry& other);

    // Default constructor
    hkGeometry() {
        m_vertices.m_capacityAndFlags = 0x80000000;
        m_vertices.m_data = nullptr;
        m_vertices.m_size = 0;
        m_triangles.m_data = nullptr;
        m_triangles.m_size = 0;
        m_triangles.m_capacityAndFlags = 0x80000000;
    }

    // IDA: ?isValid@hkGeometry@@QEBA?AVhkBool@@XZ @ 0x14079e200
    // Validates vertices (no NaN) and triangles (indices in range)
    hkBool isValid() const;

    // IDA: ?clear@hkGeometry@@QEAAXXZ @ 0x14079e290
    // Clears vertices and triangles
    void clear() {
        m_triangles.m_size = 0;
        m_vertices.m_size = 0;
    }

    // IDA: ?appendGeometry@hkGeometry@@QEAAXAEBU1@PEBVhkMatrix4@@@Z @ 0x14079e430
    // Appends another geometry transformed by matrix
    void appendGeometry(const hkGeometry& other, const hkMatrix4* transform);
};

static_assert(sizeof(hkGeometry::Triangle) == 16, "hkGeometry::Triangle size mismatch - expected 16 bytes");

