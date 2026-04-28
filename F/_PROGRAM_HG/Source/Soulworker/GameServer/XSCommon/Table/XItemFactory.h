// XItemFactory.h
// ControlServer 物品工厂 Stub

#pragma once

#include <cstdint>
#include "Soulworker/GameServer/XCore/XServer/XSeed.h"

// 对齐 ControlServer.exe IDA
class XItemFactory {
public:
    static XItemFactory& Instance() {
        static XItemFactory instance;
        return instance;
    }

    XItemFactory() = default;
    ~XItemFactory() = default;

    // 对齐 IDA: XItemFactory::Init
    static void Init(XItemFactory* pFactory, std::uint8_t byGroupID, std::uint8_t byChannel) {
        // TODO: 对齐 IDA 实现 - 初始化物品工厂
    }

    // 对齐 IDA 0x1400AD290: XItemFactory::nRand - 随机数生成
    int nRand(int nMin, int nMax) {
        if (nMin == nMax) {
            return nMin;
        }
        int nLow = nMin;
        int nHigh = nMax;
        if (nMin > nMax) {
            nLow = nMax;
            nHigh = nMin;
        }
        // 使用 XSeed::GetSeed 生成随机数
        double fRand = XSeed::GetSeed(&m_xSeed) * static_cast<double>(nHigh - nLow + 1) + static_cast<double>(nLow);
        int nResult = static_cast<int>(fRand);
        if (nResult > nHigh) {
            return nHigh;
        }
        return nResult;
    }

    // 物品创建相关
    // TODO: 对齐 IDA 实现

private:
    XSeed m_xSeed;  // 对齐 IDA: 随机种子
};