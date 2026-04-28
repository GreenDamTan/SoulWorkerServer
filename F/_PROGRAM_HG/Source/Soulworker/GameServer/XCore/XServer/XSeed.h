// XSeed.h
// ControlServer Random Seed Stub

#pragma once

#include <cstdint>
#include <random>

// 对齐 ControlServer.exe IDA
class XSeed {
public:
    XSeed() : m_engine(std::random_device{}()) {}
    ~XSeed() = default;

    // 对齐 IDA: XSeed::Init(XSeed* this, int seed)
    static void Init(XSeed* pSeed, int nSeed) {
        // 使用种子初始化随机引擎
        pSeed->m_engine.seed(static_cast<std::mt19937::result_type>(nSeed));
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
