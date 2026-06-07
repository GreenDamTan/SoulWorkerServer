// XSeed.h
// Random Seed Implementation - 用于服务器随机数生成

#pragma once

#include <cstdint>
#include <random>
#include <ctime>

// XSeed - 随机种子类，用于游戏服务器随机数生成
// 从 IDA GameServer.exe 反编译还原
class XSeed {
public:
    XSeed() : m_engine(std::random_device{}()) {}
    ~XSeed() = default;

    // 带种子的构造函数 - IDA: XSeed::XSeed(int seed)
    explicit XSeed(int nSeed) : m_engine(static_cast<std::mt19937::result_type>(nSeed)) {}

    // 初始化 - IDA: XSeed::Init(XSeed* this, int seed)
    void Init(int nSeed) {
        m_engine.seed(static_cast<std::mt19937::result_type>(nSeed));
    }

    // 静态初始化方法
    static void Init(XSeed* pSeed, int nSeed) {
        pSeed->Init(nSeed);
    }

    // 获取随机种子值 [0.0, 1.0) - IDA: XSeed::GetSeed
    double GetSeed() {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(m_engine);
    }

    static double GetSeed(XSeed* pSeed) {
        return pSeed->GetSeed();
    }

    // 根据索引获取随机数 - IDA: XSeed::GetSeedByIndex
    double GetSeedByIndex(int nIndex) {
        std::mt19937 tempEngine(m_engine() + static_cast<std::mt19937::result_type>(nIndex));
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(tempEngine);
    }

    static double GetSeedByIndex(XSeed* pSeed, int nIndex) {
        return pSeed->GetSeedByIndex(nIndex);
    }

    // 生成下一个随机数
    std::uint32_t Next() {
        return static_cast<std::uint32_t>(m_engine());
    }

    // 生成 [0, max) 范围内的随机数
    std::uint32_t Next(std::uint32_t max) {
        std::uniform_int_distribution<std::uint32_t> dist(0, max - 1);
        return dist(m_engine);
    }

    // 整数随机 [min, max] - IDA: XRand<int>
    int Rand(int nMin, int nMax) {
        std::uniform_int_distribution<int> dist(nMin, nMax);
        return dist(m_engine);
    }

    // 浮点随机 [min, max) - IDA: XRand<float>
    float Rand(float fMin, float fMax) {
        std::uniform_real_distribution<float> dist(fMin, fMax);
        return dist(m_engine);
    }

private:
    std::mt19937 m_engine;
};

// XRand<T> 模板函数 - IDA 精确还原
// IDA: ??$XRand@H@@YAHAEAVXSeed@@HH@Z (0x1402E7570)
// IDA: ??$XRand@M@@YAMAEAVXSeed@@MM@Z (0x1402E7630)
template <typename T>
T XRand(XSeed* pSeed, T min, T max);

// 整数特化 - IDA 0x1402E7570
template <>
inline int XRand<int>(XSeed* pSeed, int min, int max) {
    if (min == max) return min;
    int mina = min;
    int maxa = max;
    if (min > max) {
        mina = max;
        maxa = min;
    }
    int result = static_cast<int>(static_cast<double>(maxa - mina + 1) * pSeed->GetSeed() + static_cast<double>(mina));
    if (result > maxa) return maxa;
    return result;
}

// 浮点数特化 - IDA 0x1402E7630
template <>
inline float XRand<float>(XSeed* pSeed, float min, float max) {
    if (min == max) return min;
    float mina = min;
    float maxa = max;
    if (min > max) {
        mina = max;
        maxa = min;
    }
    float result = (maxa - mina + 1.0f) * static_cast<float>(pSeed->GetSeed()) + mina;
    if (result > maxa) return maxa;
    return result;
}
