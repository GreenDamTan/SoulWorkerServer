#pragma once

#include <cstdint>
#include <mutex>

/**
 * @brief `TXSingleton<T>` 线程安全单例模板。
 *
 * IDA 0x140001450 TXSingleton<XGameServer>::Instance 反编译:
 * ```cpp
 * XGameServer *__fastcall TXSingleton<XGameServer>::Instance()
 * {
 *   if ( !TXSingleton<XGameServer>::_pInstance )
 *   {
 *     v1 = (XGameServer *)VBaseObject::operator new(0x42438u);  // 分配 271416 字节
 *     if ( v1 )
 *       v2 = XGameServer::XGameServer(v1);  // 调用构造函数
 *     else
 *       v2 = nullptr;
 *     TXSingleton<XGameServer>::_pInstance = v2;
 *   }
 *   return TXSingleton<XGameServer>::_pInstance;
 * }
 * ```
 *
 * 关键发现:
 * - 原版使用 VBaseObject::operator new 分配内存（虚基类）
 * - XGameServer 实例大小为 0x42438 (271416 字节)
 * - 非线程安全的 lazy 初始化
 * - 无析构清理（进程生命周期单例）
 *
 * TODO: 需要验证 VBaseObject 是否有特殊内存管理逻辑
 * TODO: 原版没有线程保护，多线程环境可能需要加锁
 */
template <typename T>
class TXSingleton {
public:
    /**
     * @brief 获取单例实例。
     *
     * 对齐 IDA: 非线程安全的 lazy 初始化。
     * 原版使用 VBaseObject::operator new，当前使用标准 new。
     *
     * @return T* 单例实例指针
     *
     * TODO: 原版使用 VBaseObject::operator new，可能有自定义内存对齐
     * TODO: 如果多线程访问，需要添加 std::call_once 或 mutex 保护
     */
    static T* Instance() {
        if (!_pInstance) {
            // TODO: 对齐 IDA - 原版使用 VBaseObject::operator new
            // 当前使用标准 new，如果 T 有虚基类需要验证布局
            _pInstance = new T();
        }
        return _pInstance;
    }

private:
    /**
     * @brief 静态实例指针。
     *
     * 对齐 IDA: TXSingleton<T>::_pInstance 是静态成员。
     * 原版无清理逻辑，进程结束时由 OS 回收。
     */
    static T* _pInstance;
};

// 静态成员初始化
template <typename T>
T* TXSingleton<T>::_pInstance = nullptr;
