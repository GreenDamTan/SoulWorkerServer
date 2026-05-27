// GroupAggro.cpp
// CGroupAggro - 群体仇恨管理类实现
// IDA 反编译还原

#include "Soulworker/GameServer/XGameServer/GroupAggro.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <vector>
#include <algorithm>

// ============================================================================
// CGroupAggro 构造函数
// IDA: ??0CGroupAggro@@QEAA@XZ @ 0x1401989A0
// ============================================================================
CGroupAggro::CGroupAggro()
    : m_bIsAggro(false)
    , m_pMonster(nullptr)
    , m_nGroupID(0)
    , m_nDistance(0)
    , m_nMaxCount(0)
{
}

// ============================================================================
// CGroupAggro::Reset - 重置群体仇恨状态
// IDA: ?Reset@CGroupAggro@@QEAAXXZ @ 0x1401989E0
// ============================================================================
void CGroupAggro::Reset()
{
    m_bIsAggro = false;
    m_nGroupID = 0;
    m_nDistance = 0;
    m_nMaxCount = 0;
}

// ============================================================================
// CGroupAggro::Init - 初始化群体仇恨对象
// IDA: ?Init@CGroupAggro@@QEAAXPEAVCMonster@@@Z @ 0x140198A20
// ============================================================================
void CGroupAggro::Init(CMonster* pMonster)
{
    m_pMonster = pMonster;
}

// ============================================================================
// CGroupAggro::SetInfo - 设置群体仇恨参数
// IDA: ?SetInfo@CGroupAggro@@QEAAXHHH@Z @ 0x140198A40
// 参数:
//   _nGroupID   - 群体ID
//   _nDistance  - 触发距离
//   _nMaxCount  - 最大触发数量
// ============================================================================
void CGroupAggro::SetInfo(int _nGroupID, int _nDistance, int _nMaxCount)
{
    m_nGroupID = _nGroupID;
    m_nDistance = _nDistance;
    m_nMaxCount = _nMaxCount;

    // 只有当群体ID和最大数量都有效时才激活群体仇恨
    if (_nGroupID != 0 && _nMaxCount != 0)
    {
        m_bIsAggro = true;
    }
}

// ============================================================================
// CGroupAggro::ClearAggroFlag - 清除群体仇恨标志
// IDA: ?ClearAggroFlag@CGroupAggro@@QEAAXXZ @ 0x140198DB0
// ============================================================================
void CGroupAggro::ClearAggroFlag()
{
    m_bIsAggro = false;
}

// ============================================================================
// CGroupAggro::RunAggro - 执行群体仇恨触发
// IDA: ?RunAggro@CGroupAggro@@QEAAXXZ @ 0x140198A90
//
// 功能: 当怪物触发仇恨时，通知附近同组的其他怪物一起进入战斗状态
// ============================================================================
void CGroupAggro::RunAggro()
{
    // TODO: Implement group aggro trigger logic
    // This would scan nearby monsters with the same GroupID and trigger them
}

// ============================================================================
// Aggro Tracking Functions Implementation
// ============================================================================

// ============================================================================
// CGroupAggro::AddAggro - 为目标添加仇恨值
// ============================================================================
void CGroupAggro::AddAggro(UXActorID targetActor, float fAggro)
{
    if (targetActor.dwActorID == 0 || targetActor.dwActorID == 0xFFFFFFFFu)
    {
        return; // Invalid actor ID
    }
    
    auto it = m_aggroMap.find(targetActor);
    if (it != m_aggroMap.end())
    {
        it->second += fAggro;
    }
    else
    {
        m_aggroMap[targetActor] = fAggro;
    }
}

// ============================================================================
// CGroupAggro::RemoveAggro - 移除目标的仇恨记录
// ============================================================================
void CGroupAggro::RemoveAggro(UXActorID targetActor)
{
    m_aggroMap.erase(targetActor);
}

// ============================================================================
// CGroupAggro::Update - 更新仇恨值 (随时间衰减)
// ============================================================================
void CGroupAggro::Update(float fDeltaTime)
{
    if (m_fAggroDecayRate <= 0.0f)
    {
        return; // No decay
    }
    
    float fDecayMultiplier = 1.0f - (m_fAggroDecayRate * fDeltaTime);
    if (fDecayMultiplier < 0.0f)
    {
        fDecayMultiplier = 0.0f;
    }
    
    std::vector<UXActorID> toRemove;
    
    for (auto& pair : m_aggroMap)
    {
        pair.second *= fDecayMultiplier;
        if (pair.second < 0.01f) // Remove very low aggro
        {
            toRemove.push_back(pair.first);
        }
    }
    
    for (const auto& actor : toRemove)
    {
        m_aggroMap.erase(actor);
    }
}

// ============================================================================
// CGroupAggro::GetTopAggro - 获取仇恨值最高的目标
// ============================================================================
UXActorID CGroupAggro::GetTopAggro() const
{
    if (m_aggroMap.empty())
    {
        return UXActorID(0); // Return invalid actor ID
    }
    
    UXActorID topActor(0);
    float maxAggro = -1.0f;
    
    for (const auto& pair : m_aggroMap)
    {
        if (pair.second > maxAggro)
        {
            maxAggro = pair.second;
            topActor = pair.first;
        }
    }
    
    return topActor;
}

// ============================================================================
// CGroupAggro::Clear - 清除所有仇恨记录
// ============================================================================
void CGroupAggro::Clear()
{
    m_aggroMap.clear();
}

// ============================================================================
// CGroupAggro::GetAggroValue - 获取指定目标的仇恨值
// ============================================================================
float CGroupAggro::GetAggroValue(UXActorID targetActor) const
{
    auto it = m_aggroMap.find(targetActor);
    if (it != m_aggroMap.end())
    {
        return it->second;
    }
    return 0.0f;
}

// ============================================================================
// CGroupAggro::GetAggroList - 获取所有仇恨记录列表
// ============================================================================
std::vector<std::pair<UXActorID, float>> CGroupAggro::GetAggroList() const
{
    std::vector<std::pair<UXActorID, float>> result;
    result.reserve(m_aggroMap.size());
    
    for (const auto& pair : m_aggroMap)
    {
        result.push_back(pair);
    }
    
    // Sort by aggro value descending
    std::sort(result.begin(), result.end(),
        [](const std::pair<UXActorID, float>& a, const std::pair<UXActorID, float>& b) {
            return a.second > b.second;
        });
    
    return result;
}

// ============================================================================
// CGroupAggro::SetAggroDecay - 设置仇恨衰减率
// ============================================================================
void CGroupAggro::SetAggroDecay(float fRate)
{
    m_fAggroDecayRate = fRate;
    if (m_fAggroDecayRate < 0.0f)
    {
        m_fAggroDecayRate = 0.0f;
    }
}

// ============================================================================
// CGroupAggro::GetTargetCount - 获取有仇恨值的目标数量
// ============================================================================
size_t CGroupAggro::GetTargetCount() const
{
    return m_aggroMap.size();
}
