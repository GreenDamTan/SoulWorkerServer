#pragma once

// TXSingleton - 线程安全单例模板
// 对齐 IDA: 使用静态指针成员 + 堆分配 (非 local static)

template <typename T>
class TXSingleton {
public:
    static T* Instance() {
        if (!_pInstance) {
            _pInstance = new T();
        }
        return _pInstance;
    }

private:
    static T* _pInstance;
};

// 静态成员初始化
template <typename T>
T* TXSingleton<T>::_pInstance = nullptr;
