#pragma once

template <typename T>
class TXSingleton {
public:
    static T* Instance() {
        static T instance;
        return &instance;
    }
};
