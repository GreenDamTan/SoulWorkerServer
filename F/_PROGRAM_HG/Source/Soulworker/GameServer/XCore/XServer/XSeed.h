// XSeed.h
// Random Seed Implementation - PDB layout recovered from GameServer.exe

#pragma once

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <memory>

class XSeed {
public:
    XSeed() {
        m_nMaxSeedCount = 1000000;
    }

    explicit XSeed(int nSeed) : XSeed() {
        m_nDum = nSeed;
    }

    ~XSeed() = default;

    void Init(int nSeed) {
        SetSeed(nSeed != 0, false);
    }

    static void Init(XSeed* pSeed, int nSeed) {
        if (pSeed) {
            pSeed->Init(nSeed);
        }
    }

    void SetSeed(bool seedFromTime, bool unused = false) {
        (void)unused;
        m_nDum = 123456789;
        m_nDum2 = 123456789;
        m_nIy = 0;
        std::fill(std::begin(m_nIv), std::end(m_nIv), 0);
        m_pArray.reset();
        m_nCurrIndex = 0;
        m_nCurrMaxIndex = 0;
        m_bUseArray = false;
        m_nMaxSeedCount = 1000000;

        unsigned int srandSeed = 0;
        if (seedFromTime) {
            srandSeed = static_cast<unsigned int>(std::time(nullptr));
        }
        std::srand(srandSeed);

        int value = 0;
        do {
            value = std::rand();
        } while (value == 0);
        m_nDum = -value;
    }

    int GenTableForNumbers(bool seedFromTime, bool unused = false) {
        SetSeed(seedFromTime, unused);
        if (m_nMaxSeedCount <= 0) {
            m_bUseArray = true;
            m_nCurrMaxIndex = m_nMaxSeedCount;
            return 0;
        }

        m_pArray = std::make_unique<double[]>(
            static_cast<std::size_t>(m_nMaxSeedCount));
        for (int index = 0; index < m_nMaxSeedCount; ++index) {
            if (m_nDum <= 0) {
                int value = static_cast<int>(
                    std::max<std::int64_t>(1, -static_cast<std::int64_t>(m_nDum)));
                m_nDum = value;
                m_nDum2 = value;
                for (int slot = 39; slot >= 0; --slot) {
                    m_nDum = SchrageStep(m_nDum, 40014, 2147483563, 53668);
                    if (slot < 32) {
                        m_nIv[slot] = m_nDum;
                    }
                }
                m_nIy = m_nIv[0];
            }

            m_nDum = SchrageStep(m_nDum, 40014, 2147483563, 53668);
            m_nDum2 = SchrageStep(m_nDum2, 40692, 2147483399, 52774);

            const int tableIndex = static_cast<int>(
                static_cast<double>(m_nIy) / 67108862.0);
            m_nIy = m_nIv[tableIndex] - m_nDum2;
            m_nIv[tableIndex] = m_nDum;
            if (m_nIy < 1) {
                m_nIy += 2147483562;
            }

            double value = static_cast<double>(m_nIy) * 4.656613057391769e-10;
            if (value > 0.99999988) {
                value = 0.99999988;
            }
            m_pArray[index] = value;
        }

        m_nCurrIndex = 0;
        m_bUseArray = true;
        m_nCurrMaxIndex = m_nMaxSeedCount;
        return 0;
    }

    double GetSeed() {
        if (!m_pArray || !m_bUseArray || m_nCurrMaxIndex <= 0) {
            GenTableForNumbers(false, false);
        }
        int currentIndex = m_nCurrIndex;
        if (currentIndex >= m_nMaxSeedCount) {
            currentIndex = 0;
        }
        m_nCurrIndex = currentIndex;
        const double result = m_pArray
            ? m_pArray[static_cast<std::size_t>(currentIndex)]
            : 0.0;
        int nextIndex = currentIndex + 1;
        if (nextIndex >= m_nCurrMaxIndex) {
            nextIndex = 0;
        }
        m_nCurrIndex = nextIndex;
        return result;
    }

    static double GetSeed(XSeed* pSeed) {
        return pSeed ? pSeed->GetSeed() : 0.0;
    }

    double GetSeedByIndex(int nIndex) {
        if (!m_pArray || !m_bUseArray || m_nCurrMaxIndex <= 0) {
            GenTableForNumbers(false, false);
        }
        if (nIndex < 0 || nIndex >= m_nCurrMaxIndex || !m_pArray) {
            return 0.0;
        }
        return m_pArray[static_cast<std::size_t>(nIndex)];
    }

    static double GetSeedByIndex(XSeed* pSeed, int nIndex) {
        return pSeed ? pSeed->GetSeedByIndex(nIndex) : 0.0;
    }

    std::uint32_t Next() {
        return static_cast<std::uint32_t>(GetSeed() * 4294967296.0);
    }

    std::uint32_t Next(std::uint32_t max) {
        return max == 0 ? 0 : Next() % max;
    }

    int Rand(int nMin, int nMax) {
        if (nMin == nMax) {
            return nMin;
        }
        int minValue = nMin;
        int maxValue = nMax;
        if (nMin > nMax) {
            minValue = nMax;
            maxValue = nMin;
        }
        int result = static_cast<int>(
            static_cast<double>(maxValue - minValue + 1) * GetSeed() + minValue);
        return result > maxValue ? maxValue : result;
    }

    float Rand(float fMin, float fMax) {
        if (fMin == fMax) {
            return fMin;
        }
        float minValue = fMin;
        float maxValue = fMax;
        if (fMin > fMax) {
            minValue = fMax;
            maxValue = fMin;
        }
        float result = (maxValue - minValue + 1.0f) *
                       static_cast<float>(GetSeed()) + minValue;
        return result > maxValue ? maxValue : result;
    }

private:
    static int SchrageStep(int current, int multiplier, int modulus, int divisor) {
        const std::int64_t quotient = current / divisor;
        const std::int64_t next = static_cast<std::int64_t>(multiplier) * current -
                                  static_cast<std::int64_t>(modulus) * quotient;
        return static_cast<int>(next < 0 ? next + modulus : next);
    }

    int m_nDum = 123456789;
    int m_nDum2 = 123456789;
    int m_nIy = 0;
    int m_nIv[32] = {};
    std::unique_ptr<double[]> m_pArray;
    int m_nCurrIndex = 0;
    int m_nCurrMaxIndex = 0;
    bool m_bUseArray = false;
    std::uint8_t m_padding[3] = {};
    int m_nMaxSeedCount = 1000000;
};

static_assert(sizeof(XSeed) == 168, "XSeed size must match GameServer PDB");

template <typename T>
T XRand(XSeed* pSeed, T min, T max);

template <>
inline int XRand<int>(XSeed* pSeed, int min, int max) {
    if (min == max) {
        return min;
    }
    int minValue = min;
    int maxValue = max;
    if (min > max) {
        minValue = max;
        maxValue = min;
    }
    int result = static_cast<int>(
        static_cast<double>(maxValue - minValue + 1) * pSeed->GetSeed() + minValue);
    return result > maxValue ? maxValue : result;
}

template <>
inline float XRand<float>(XSeed* pSeed, float min, float max) {
    if (min == max) {
        return min;
    }
    float minValue = min;
    float maxValue = max;
    if (min > max) {
        minValue = max;
        maxValue = min;
    }
    float result = (maxValue - minValue + 1.0f) *
                   static_cast<float>(pSeed->GetSeed()) + minValue;
    return result > maxValue ? maxValue : result;
}
