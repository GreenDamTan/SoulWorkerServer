// XWorldManager.h
// World Manager - 管理世界/地图实例

#pragma once

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/XSeed.h"
#include "Soulworker/GameServer/XCore/XServer/CFSRWLock.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <cstdint>
#include <map>
#include <functional>

// 前置声明
class XArea;

/**
 * @brief XWorldManager - 世界管理器
 *
 * 对齐 IDA 分析结果:
 * - TXSingleton<XWorldManager>::Instance @ 0x14000e080
 * - 实例大小: 0x198 (408 字节)
 * - 使用 VBaseObject::operator new 分配内存
 *
 * 构造函数 @ 0x1407193E0:
 *   std::tr1::_Callable_base<...>::_Get(&this->m_nStaticMapCount);
 *   this->__vftable = &XWorldManager::`vftable';
 *   std::map<E_FSMSTATES, CFsmState<CAi>*> m_mapClientArea;  // 初始化为空 map
 *   this->m_bReqWorldIInfo = 0;
 *   XSeed::XSeed(&this->m_xSeed, 0);
 *   XSeed::XSeed(&this->m_xTrapSeed, 0);
 *   this->m_nSerial = -1;
 *   this->m_bInit = 0;
 *   this->m_bFinishLoad = 0;
 *   CFSRWLock::CFSRWLock(&this->m_rwLock);
 *
 * 关键成员布局 (从构造函数推断):
 * - 偏移 0x00: vtable (继承自 VBaseObject)
 * - 偏移 0x08: m_funcStorage (std::tr1::function 存储)
 * - 偏移 0x30: m_mapClientArea (std::map)
 * - 偏移 ~0x48: m_bReqWorldIInfo (bool)
 * - 偏移 ~0x4C: m_xSeed (XSeed)
 * - 偏移 ~0x70: m_xTrapSeed (XSeed)
 * - 偏移 ~0x94: m_nSerial (int)
 * - 偏移 ~0x98: m_bInit (bool)
 * - 偏移 ~0x99: m_bFinishLoad (bool)
 * - 偏移 ~0x9A: m_rwLock (CFSRWLock)
 * - 偏移 ~0xBC: m_nStaticMapCount (int)
 */
class XWorldManager : public VBaseObject {
public:
    // 构造/析构
    XWorldManager();
    virtual ~XWorldManager();

    // 单例访问 - 通过 TXSingleton 实现
    static XWorldManager* Instance() {
        return TXSingleton<XWorldManager>::Instance();
    }

    // 初始化/清理
    void Init();
    void Clear();
    bool IsInit();

    // 世界信息请求
    void ReqWorldInfo();

    // 随机数生成
    int RandProb();                          // 返回 1-10000 随机数
    int nRand(int nMin, int nMax);           // 整数范围随机
    std::int64_t nTrapRand(int nIndex);      // 陷阱随机（基于索引）
    float fRand(float fMin, float fMax);     // 浮点范围随机

    // 区域管理
    bool CreatChannleDistrict();
    bool CreatChannleBattleCry();
    void AddArea(XArea* pArea);

    // 虚函数
    virtual void GetSpawnPos(const void* pSpawnInfo, XVec3& vPos);
    virtual void OnUpdate(std::uint64_t dwTick);

private:
    // 成员变量 - 对齐 IDA 0x198 字节布局
    // 注意: 当前布局是推断结果，精确偏移需要进一步验证

    // std::tr1::function 成员存储空间
    // 构造函数中调用 std::tr1::_Callable_base::_Get 初始化
    // 使用 std::function 替代 std::tr1::function
    std::function<void()> m_callback;

    // m_mapClientArea: std::map<E_FSMSTATES, CFsmState<CAi>*>
    // 构造函数中初始化为空 map
    // 使用 int 作为 E_FSMSTATES 的占位符
    std::map<int, void*> m_mapClientArea;

    bool m_bReqWorldIInfo = false;       // 是否请求世界信息
    XSeed m_xSeed;                       // 主随机数生成器
    XSeed m_xTrapSeed;                   // 陷阱随机数生成器
    int m_nSerial = -1;                  // 序列号 (初始 -1, Init 后从 100 开始)
    bool m_bInit = false;                // 是否已初始化
    bool m_bFinishLoad = false;          // 是否加载完成 (IsInit 返回此值)
    CFSRWLock m_rwLock;                  // 读写锁
    int m_nStaticMapCount = 0;           // 静态地图计数
};
