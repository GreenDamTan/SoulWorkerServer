#pragma once

#include <cstdint>

/**
 * @brief `VBaseObject` 虚基类最小还原。
 *
 * IDA 反编译 VBaseObject::operator new @ 0x140189880:
 * ```cpp
 * void *__fastcall VBaseObject::operator new(unsigned __int64 iSize)
 * {
 *   return VBaseAlloc_rel(iSize);  // 引擎自定义内存分配器
 * }
 * ```
 *
 * VBaseObject::VBaseObject @ 0x14063FE60:
 * ```cpp
 * VBaseObject *__fastcall VBaseObject::VBaseObject(VBaseObject *this)
 * {
 *   this->__vftable = (VBaseObject_vtbl *)&VBaseObject::`vftable';
 *   return this;
 * }
 * ```
 *
 * 关键发现:
 * - VBaseObject 是虚基类，有 vtable
 * - operator new 调用引擎的 VBaseAlloc_rel 分配器
 * - VBaseAlloc_rel 来自外部引擎库（Base.dll）
 *
 * TODO: VBaseAlloc_rel 的具体实现需要引擎源码
 * 当前使用标准 new 作为替代
 */
class VBaseObject {
public:
    /**
     * @brief 自定义内存分配。
     *
     * 对齐 IDA 0x140189880: 原版调用 VBaseAlloc_rel
     * 当前使用标准 malloc 作为替代实现
     *
     * @param iSize 分配大小
     * @return void* 分配的内存指针
     */
    static void* operator new(std::size_t iSize) {
        // TODO: 对齐原版 VBaseAlloc_rel(iSize)
        // 当前使用标准 malloc，如果有对齐需求可改为 aligned_alloc
        return ::operator new(iSize);
    }

    /**
     * @brief 自定义内存释放。
     *
     * @param ptr 要释放的指针
     */
    static void operator delete(void* ptr) noexcept {
        ::operator delete(ptr);
    }

    /**
     * @brief 虚析构函数。
     *
     * IDA 0x140640050: VBaseObject::~VBaseObject
     */
    virtual ~VBaseObject() = default;

protected:
    /**
     * @brief 默认构造函数。
     *
     * IDA 0x14063FE60: 设置 vtable 指针
     */
    VBaseObject() = default;
};

/**
 * @brief `TXSingleton<T>` 单例模板。
 *
 * IDA 反编译的所有 TXSingleton<T>::Instance 函数遵循相同模式:
 *
 * TXSingleton<XGameServer>::Instance @ 0x140001450:
 * ```cpp
 * XGameServer *__fastcall TXSingleton<XGameServer>::Instance()
 * {
 *   XGameServer *v1;
 *   XGameServer *v2;
 *   if ( !TXSingleton<XGameServer>::_pInstance )
 *   {
 *     v1 = (XGameServer *)VBaseObject::operator new(0x42438u);  // 271416 字节
 *     if ( v1 )
 *       v2 = XGameServer::XGameServer(v1);
 *     else
 *       v2 = nullptr;
 *     TXSingleton<XGameServer>::_pInstance = v2;
 *   }
 *   return TXSingleton<XGameServer>::_pInstance;
 * }
 * ```
 *
 * TXSingleton<XWorldManager>::Instance @ 0x14000e080:
 * - 实例大小: 0x198 (408 字节)
 *
 * TXSingleton<CCalculateStatus>::Instance @ 0x140045540:
 * - 实例大小: 0x9C0 (2496 字节)
 *
 * TXSingleton<CLogicThreadManager>::Instance @ 0x1400e9d30:
 * - 实例大小: 0x108 (264 字节)
 *
 * TXSingleton<CGameLogThreadManager>::Instance @ 0x1401ee400:
 * - 实例大小: 0x68 (104 字节)
 *
 * 关键特征:
 * - 非线程安全的 lazy 初始化
 * - 使用 VBaseObject::operator new 分配内存
 * - 无析构清理（进程生命周期单例）
 * - 构造函数在分配成功后调用
 *
 * @tparam T 单例类型，必须继承自 VBaseObject
 */
template <typename T>
class TXSingleton {
public:
    /**
     * @brief 获取单例实例。
     *
     * 对齐 IDA: 非线程安全的 lazy 初始化模式。
     *
     * 实现步骤（严格对齐 IDA）:
     * 1. 检查 _pInstance 是否为空
     * 2. 如果为空，调用 VBaseObject::operator new 分配内存
     * 3. 如果分配成功，调用 T 的构造函数
     * 4. 将结果存入 _pInstance
     * 5. 返回 _pInstance
     *
     * @return T* 单例实例指针
     *
     * NOTE: 原版非线程安全，多线程首次访问可能创建多个实例
     * TODO: 如果需要线程安全，应使用 std::call_once 或 mutex
     */
    static T* Instance() {
        if (!_pInstance) {
            // 对齐 IDA: VBaseObject::operator new(sizeof(T))
            // 先分配内存
            void* mem = VBaseObject::operator new(sizeof(T));
            if (mem) {
                // 对齐 IDA: 调用构造函数
                // 使用 placement new 在已分配内存上构造对象
                _pInstance = new (mem) T();
            }
            // 对齐 IDA: 即使构造失败也设置 _pInstance（原版逻辑）
            // 如果构造函数抛异常，_pInstance 保持 nullptr
        }
        return _pInstance;
    }

    /**
     * @brief 检查单例是否已创建。
     *
     * @return bool true 如果单例已创建
     */
    static bool IsCreated() {
        return _pInstance != nullptr;
    }

protected:
    /**
     * @brief 静态实例指针。
     *
     * 对齐 IDA: TXSingleton<T>::_pInstance 是静态成员。
     * 原版无清理逻辑，进程结束时由 OS 回收。
     *
     * 符号名（IDA mangled）:
     * - TXSingleton<XGameServer>::_pInstance -> ?_pInstance@?$TXSingleton@VXGameServer@@@@0PEAVXGameServer@@EA
     */
    static T* _pInstance;
};

// 静态成员初始化
template <typename T>
T* TXSingleton<T>::_pInstance = nullptr;
