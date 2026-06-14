// GroupAggro.cpp
// CGroupAggro - 群体仇恨管理类实现
// IDA 反编译还原

#include "Soulworker/GameServer/XGameServer/GroupAggro.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XCore/XArea/XMaze.h"
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
// IDA 精确还原
// ============================================================================
void CGroupAggro::RunAggro() {
    // IDA: 检查是否激活了群体仇恨
    if (!m_bIsAggro || !m_pMonster) {
        return;
    }

    // IDA: 获取怪物所在的区域
    XArea* pArea = m_pMonster->GetArea();
    if (!pArea) {
        return;
    }

    // IDA: RTTI dynamic_cast 检查是否为 XMaze
    XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
    if (!pMaze) {
        return;
    }

    // IDA: 获取当前目标ID
    std::uint32_t dwTargetID = m_pMonster->GetTargetID();
    if (dwTargetID == 0xFFFFFFFF) {
        return;
    }

    // IDA: 扫描附近对象
    std::vector<CMover*> vecGameObjList;
    XArea::ScanGridOrigin(m_pMonster, 2, 2, vecGameObjList);

    // IDA: 遍历扫描结果
    int nAggroCount = 0;
    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        CMover* pMover = *it;
        if (!pMover) {
            continue;
        }

        // IDA: 检查是否为怪物类型 (type == 2)
        E_ACTOR_TYPE eType = pMover->GetType();
        if (eType != eActorMonster) {
            continue;
        }

        // IDA: RTTI dynamic_cast 到 CMonster
        CMonster* pOtherMonster = dynamic_cast<CMonster*>(pMover);
        if (!pOtherMonster) {
            continue;
        }

        // IDA: 获取其他怪物的群体仇恨对象
        CGroupAggro* pOtherGroupAggro = pOtherMonster->GetGroupAggro();
        if (!pOtherGroupAggro) {
            continue;
        }

        // IDA: 比较GroupID (VObjectComponentCollection::Count 实际上是获取 m_nGroupID)
        if (pOtherGroupAggro->m_nGroupID != this->m_nGroupID) {
            continue;
        }

        // IDA: 计算距离
        hkvVec3 vMyPos = m_pMonster->GetPosition();
        hkvVec3 vOtherPos = pOtherMonster->GetPosition();
        hkvVec3 vDiff = vMyPos - vOtherPos;
        float fDistance = vDiff.GetLength();

        // IDA: 检查是否在触发距离内
        if ((float)m_nDistance <= fDistance) {
            continue;
        }

        // IDA: 检查其他怪物是否已有目标
        std::uint32_t dwOtherTargetID = pOtherMonster->GetTargetID();
        if (dwOtherTargetID == 0xFFFFFFFF) {
            // IDA: 应用仇恨值
            pOtherMonster->ApplyAggroValue(dwTargetID, 1.0f, true);
            // IDA: 伤害激怒处理
            pOtherMonster->DamageAggressive();
            // IDA: 改变AI状态为战斗 (3)
            pOtherMonster->ChangeAiState(3);
            // IDA: 清除群体仇恨标志
            pOtherGroupAggro->ClearAggroFlag();
        }

        // IDA: 增加触发计数
        ++nAggroCount;

        // IDA: 检查是否达到最大触发数量
        if (nAggroCount >= m_nMaxCount) {
            break;
        }
    }

    // IDA: 清除自己的群体仇恨标志
    m_bIsAggro = false;
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
