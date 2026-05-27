#pragma once

#include <cstdint>
#include <algorithm>

// Include PSCommon.h first to get UXActorID and its hash specialization
// before unordered_map instantiates std::hash<UXActorID>
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"

#include <unordered_map>
#include <vector>

// 前置声明
class CMonster;

// ============================================================================
// CGroupAggro - 群体仇恨管理类
// IDA 确认大小: 32 bytes
//
// 功能: 管理怪物群体的仇恨联动机制
// 当一个怪物被攻击触发仇恨时，可以通知附近同组的其他怪物一起进入战斗状态
// ============================================================================
class CGroupAggro {
public:
    // === 构造函数 ===
    // IDA: ??0CGroupAggro@@QEAA@XZ @ 0x1401989A0
    CGroupAggro();

    // === 析构函数 ===
    ~CGroupAggro() = default;

    // === IDA 反编译确认的方法 ===

    // Reset - 重置群体仇恨状态
    // IDA: ?Reset@CGroupAggro@@QEAAXXZ @ 0x1401989E0
    void Reset();

    // Init - 初始化群体仇恨对象，绑定所属怪物
    // IDA: ?Init@CGroupAggro@@QEAAXPEAVCMonster@@@Z @ 0x140198A20
    void Init(CMonster* pMonster);

    // SetInfo - 设置群体仇恨参数
    // IDA: ?SetInfo@CGroupAggro@@QEAAXHHH@Z @ 0x140198A40
    // 参数:
    //   nGroupID   - 群体ID (同ID的怪物会被一起触发)
    //   nDistance  - 触发距离 (范围内的同组怪物会被触发)
    //   nMaxCount  - 最大触发数量 (一次最多触发多少只怪物)
    void SetInfo(int nGroupID, int nDistance, int nMaxCount);

    // RunAggro - 执行群体仇恨触发
    // IDA: ?RunAggro@CGroupAggro@@QEAAXXZ @ 0x140198A90
    // 功能: 扫描附近同组的怪物，使其进入战斗状态
    void RunAggro();

    // ClearAggroFlag - 清除群体仇恨标志
    // IDA: ?ClearAggroFlag@CGroupAggro@@QEAAXXZ @ 0x140198DB0
    void ClearAggroFlag();

    // === Aggro Tracking Functions ===
    
    // AddAggro - 为目标添加仇恨值
    void AddAggro(UXActorID targetActor, float fAggro);
    
    // RemoveAggro - 移除目标的仇恨记录
    void RemoveAggro(UXActorID targetActor);
    
    // Update - 更新仇恨值 (随时间衰减)
    void Update(float fDeltaTime);
    
    // GetTopAggro - 获取仇恨值最高的目标
    UXActorID GetTopAggro() const;
    
    // Clear - 清除所有仇恨记录
    void Clear();
    
    // GetAggroValue - 获取指定目标的仇恨值
    float GetAggroValue(UXActorID targetActor) const;
    
    // GetAggroList - 获取所有仇恨记录列表
    std::vector<std::pair<UXActorID, float>> GetAggroList() const;
    
    // SetAggroDecay - 设置仇恨衰减率
    void SetAggroDecay(float fRate);
    
    // GetTargetCount - 获取有仇恨值的目标数量
    size_t GetTargetCount() const;
    
    // HasAggro - 检查是否有任何仇恨值
    bool HasAggro() const;
    
    // TransferAggro - 将仇恨转移给另一个目标
    void TransferAggro(UXActorID fromActor, UXActorID toActor);
    
    // CopyAggro - 复制仇恨列表
    void CopyAggro(const CGroupAggro& other);
    
    // GetDecayRate - 获取仇恨衰减率
    float GetDecayRate() const;
    
    // PauseDecay - 暂停仇恨衰减
    void PauseDecay();
    
    // ResumeDecay - 恢复仇恨衰减
    void ResumeDecay();

    // === 成员访问器 ===
    bool IsAggro() const { return m_bIsAggro; }
    void SetAggro(bool bAggro) { m_bIsAggro = bAggro; }

    CMonster* GetMonster() const { return m_pMonster; }
    void SetMonster(CMonster* pMonster) { m_pMonster = pMonster; }

    int GetGroupID() const { return m_nGroupID; }
    void SetGroupID(int nGroupID) { m_nGroupID = nGroupID; }

    int GetDistance() const { return m_nDistance; }
    void SetDistance(int nDistance) { m_nDistance = nDistance; }

    int GetMaxCount() const { return m_nMaxCount; }
    void SetMaxCount(int nMaxCount) { m_nMaxCount = nMaxCount; }

private:
    // === IDA 确认的成员变量 ===
    // offset 0: m_bIsAggro (bool)
    bool m_bIsAggro;
    // offset 1-7: padding
    char _pad0[7];

    // offset 8: m_pMonster (CMonster*)
    CMonster* m_pMonster;

    // offset 16: m_nGroupID (int)
    int m_nGroupID;

    // offset 20: m_nDistance (int)
    int m_nDistance;

    // offset 24: m_nMaxCount (int)
    int m_nMaxCount;

    // offset 28-31: padding
    char _pad1[4];

    // Total: 32 bytes (original layout preserved)

    // === Aggro Tracking Members (non-IDB, new functionality) ===
    // 仇恨值映射表: 目标ActorID -> 仇恨值
    std::unordered_map<UXActorID, float> m_aggroMap;
    
    // 仇恨衰减率 (每秒衰减百分比)
    float m_fAggroDecayRate = 0.0f;
    
    // 仇恨衰减是否暂停
    bool m_bDecayPaused = false;
};
