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

// Forward declaration
class CBattleZone;

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
void CGroupAggro::RunAggro() {
    // 检查是否激活了群体仇恨
    if (!m_bIsAggro || !m_pMonster) {
        return;
    }

    // 检查参数是否有效
    if (m_nGroupID == 0 || m_nDistance <= 0 || m_nMaxCount <= 0) {
        return;
    }

    // 获取当前怪物的位置
    // Per IDA: Get monster position for distance check
    // Note: GetPosition() may not be available, use placeholder
    hkvVec3 vMyPos; // TODO: m_pMonster->GetPosition();
    
    // 获取怪物所在的区域
    void* pZone = m_pMonster->GetZone();
    if (!pZone) {
        return;
    }

    // 计算距离的平方用于比较
    float fDistanceSq = static_cast<float>(m_nDistance * m_nDistance);
    int nTriggeredCount = 0;

    // 遍历区域内的所有怪物，寻找同组的怪物
    // 注：实际实现需要访问CBattleZone的怪物列表
    // 这里使用简化逻辑，实际应该调用 CBattleZone::GetMonsterList 或类似方法
    
    // Per IDA: scan nearby monsters with same group ID
    // and trigger their aggro if within distance and count limit
    
    // 标记当前怪物已触发群体仇恨
    m_bIsAggro = true;
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
    if (m_bDecayPaused || m_fAggroDecayRate <= 0.0f)
    {
        return; // Decay paused or no decay
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

// ============================================================================
// CGroupAggro::HasAggro - 检查是否有任何仇恨值
// ============================================================================
bool CGroupAggro::HasAggro() const
{
    return !m_aggroMap.empty();
}

// ============================================================================
// CGroupAggro::TransferAggro - 将仇恨转移给另一个目标
// ============================================================================
void CGroupAggro::TransferAggro(UXActorID fromActor, UXActorID toActor)
{
    if (fromActor.dwActorID == 0 || toActor.dwActorID == 0 ||
        fromActor.dwActorID == 0xFFFFFFFFu || toActor.dwActorID == 0xFFFFFFFFu)
    {
        return; // Invalid actor ID
    }
    
    auto it = m_aggroMap.find(fromActor);
    if (it != m_aggroMap.end())
    {
        float fAggro = it->second;
        m_aggroMap.erase(it);
        
        // Add to target (accumulate if exists)
        auto targetIt = m_aggroMap.find(toActor);
        if (targetIt != m_aggroMap.end())
        {
            targetIt->second += fAggro;
        }
        else
        {
            m_aggroMap[toActor] = fAggro;
        }
    }
}

// ============================================================================
// CGroupAggro::CopyAggro - 复制仇恨列表
// ============================================================================
void CGroupAggro::CopyAggro(const CGroupAggro& other)
{
    m_aggroMap = other.m_aggroMap;
    m_fAggroDecayRate = other.m_fAggroDecayRate;
    m_bDecayPaused = other.m_bDecayPaused;
}

// ============================================================================
// CGroupAggro::GetDecayRate - 获取仇恨衰减率
// ============================================================================
float CGroupAggro::GetDecayRate() const
{
    return m_fAggroDecayRate;
}

// ============================================================================
// CGroupAggro::PauseDecay - 暂停仇恨衰减
// ============================================================================
void CGroupAggro::PauseDecay()
{
    m_bDecayPaused = true;
}

// ============================================================================
// CGroupAggro::ResumeDecay - 恢复仇恨衰减
// ============================================================================
void CGroupAggro::ResumeDecay()
{
    m_bDecayPaused = false;
}
