// GroupAggro.cpp
// CGroupAggro - 群体仇恨管理类实现
// IDA 反编译还原

#include "Soulworker/GameServer/XGameServer/GroupAggro.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include <vector>

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
