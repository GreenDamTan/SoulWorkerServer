// XSeed.h
// ControlServer Random Seed Stub

#pragma once

#include <cstdint>
#include <random>
#include <vector>

// 对齐 ControlServer.exe IDA
class XSeed {
public:
    XSeed() : m_engine(std::random_device{}()) {}
    ~XSeed() = default;

    // 对齐 IDA: XSeed::XSeed(XSeed* this, int seed) - 带种子的构造函数
    XSeed(int nSeed) : m_engine(static_cast<std::mt19937::result_type>(nSeed)) {}

    // 对齐 IDA: XSeed::Init(XSeed* this, int seed)
    static void Init(XSeed* pSeed, int nSeed) {
        // 使用种子初始化随机引擎
        pSeed->m_engine.seed(static_cast<std::mt19937::result_type>(nSeed));
    }

    // 对齐 IDA: XSeed::GetSeed - 返回 0-1 之间的随机浮点数
    static double GetSeed(XSeed* pSeed) {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(pSeed->m_engine);
    }

    // 对齐 IDA: XSeed::GetSeedByIndex - 根据索引获取随机数
    // 用于 nTrapRand 函数
    static double GetSeedByIndex(XSeed* pSeed, int nIndex) {
        // 使用索引作为种子生成确定性随机数
        std::mt19937 tempEngine(pSeed->m_engine() + static_cast<std::mt19937::result_type>(nIndex));
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(tempEngine);
    }

    std::uint32_t Next() {
        return static_cast<std::uint32_t>(m_engine());
    }

    std::uint32_t Next(std::uint32_t max) {
        std::uniform_int_distribution<std::uint32_t> dist(0, max - 1);
        return dist(m_engine);
    }

private:
    std::mt19937 m_engine;
};

// 对齐 IDA: XRand<T> 模板函数
// 用于生成指定范围内的随机数
template <typename T>
T XRand(XSeed* pSeed, T min, T max);

// 整数特化
template <>
inline int XRand<int>(XSeed* pSeed, int min, int max) {
    double seed = XSeed::GetSeed(pSeed);
    return static_cast<int>(min + seed * (max - min + 1));
}

// 浮点数特化
template <>
inline float XRand<float>(XSeed* pSeed, float min, float max) {
    double seed = XSeed::GetSeed(pSeed);
    return static_cast<float>(min + seed * (max - min));
}
