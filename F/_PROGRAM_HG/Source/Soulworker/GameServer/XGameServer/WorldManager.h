// XWorldManager.h
// World Manager - 管理世界/地图实例

#pragma once

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/XSeed.h"
#include "Soulworker/GameServer/XCore/XServer/CFSRWLock.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <cstdint>
#include <map>

// 前置声明
class XArea;

/**
 * @brief XWorldManager - 世界管理器
 *
 * 对齐 IDA: TXSingleton<XWorldManager>::Instance @ 0x14000e080
 * - 实例大小: 0x198 (408 字节)
 * - 使用 VBaseObject::operator new 分配内存
 *
 * 关键成员（从构造函数推断）:
 * - m_mapClientArea: std::map<int, XArea*>
 * - m_bReqWorldIInfo: bool
 * - m_xSeed: XSeed (随机数生成器)
 * - m_xTrapSeed: XSeed (陷阱随机数生成器)
 * - m_nSerial: int (序列号，初始 -1)
 * - m_bInit: bool (初始化标志)
 * - m_bFinishLoad: bool (加载完成标志)
 * - m_rwLock: CFSRWLock (读写锁)
 * - m_nStaticMapCount: int (静态地图计数)
 */
class XWorldManager : public VBaseObject {
public:
    // 构造/析构
    XWorldManager();
    virtual ~XWorldManager();

    // 单例访问
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
    // 注意: 布局需要从 IDA 验证，当前是推断结果

    // std::tr1::function 成员 (用于回调)
    // 构造函数中调用 _Callable_base::_Get 初始化
    char m_funcStorage[40];  // std::tr1::function 存储空间

    // m_mapClientArea: std::map<E_FSMSTATES, CFsmState<CAi>*>
    // 构造函数中初始化为空 map
    std::map<int, void*> m_mapClientArea;  // 占位，实际类型需验证

    bool m_bReqWorldIInfo;      // 是否请求世界信息
    XSeed m_xSeed;              // 主随机数生成器
    XSeed m_xTrapSeed;          // 陷阱随机数生成器
    int m_nSerial;              // 序列号 (初始 -1, Init 后从 100 开始)
    bool m_bInit;               // 是否已初始化
    bool m_bFinishLoad;         // 是否加载完成 (IsInit 返回此值)
    CFSRWLock m_rwLock;         // 读写锁
    int m_nStaticMapCount;      // 静态地图计数
};
