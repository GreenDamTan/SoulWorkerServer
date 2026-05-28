// GocMaze.cpp
// CGocMaze - Game Object Component for Maze system
// 从 IDA GameServer.exe 精确还原

#include "GocMaze.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XGameServer/SendPacket.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/ResourceMgr.h"
#include "Soulworker/GameServer/XGameServer/Actor/Component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/Actor/Component/GocRecode.h"
#include "Soulworker/GameServer/XGameServer/Actor/Component/GocAttribute.h"
#include <tr1/memory>

// ============================================================================
// 静态方法
// ============================================================================

// IDA: ?GetFamilyID@CGocMaze@@SAHXZ
// Family ID = 待验证 (Maze组件的家族ID)
int CGocMaze::GetFamilyID()
{
    // TODO: 需人工审查 - 需要从IDA验证具体的FamilyID值
    return 9;  // 推测值
}

// ============================================================================
// 构造函数/析构函数
// ============================================================================

// IDA: ??0CGocMaze@@QEAA@XZ
CGocMaze::CGocMaze()
    : GOComponent()
    , m_pMaze()
    , m_nEnterCount(0)
    , m_nClearCount(0)
    , m_biBestTime(0)
{
    // GOComponent 构造函数会初始化 vtable
    // m_pMaze 默认初始化为空 shared_ptr
}

// IDA: ??1CGocMaze@@UEAA@XZ
CGocMaze::~CGocMaze()
{
    // 设置 vtable 指向 CGocMaze 的虚函数表
    // 释放 m_pMaze (shared_ptr 析构)
    m_pMaze.reset();
}

// ============================================================================
// 初始化/清理
// ============================================================================

// IDA: ?Init@CGocMaze@@QEAAXXZ
void CGocMaze::Init()
{
    m_pMaze.reset();
    m_nEnterCount = 0;
    m_nClearCount = 0;
    m_biBestTime = 0;
}

// IDA: ?Clear@CGocMaze@@QEAAXXZ
void CGocMaze::Clear()
{
    if (m_pMaze)
    {
        // 获取当前玩家的 ActorID
        CMover* pOwner = GetOwnerGO();
        if (pOwner)
        {
            // TODO: 汇编还原 - 需要从迷宫中移除玩家
            // 具体逻辑需要从IDA反编译
        }

        m_pMaze.reset();
    }
    m_nEnterCount = 0;
    m_nClearCount = 0;
    m_biBestTime = 0;
}

// ============================================================================
// 迷宫状态查询
// ============================================================================

// IDA: ?IsMaze@CGocMaze@@QEAA_NXZ
bool CGocMaze::IsMaze() const
{
    // 检查 shared_ptr 是否有效 (operator bool)
    return m_pMaze.operator bool();
}

// IDA: ?GetMaze@CGocMaze@@QEAAPEAVXMaze@@XZ
XMaze* CGocMaze::GetMaze() const
{
    return m_pMaze.get();
}

// IDA: ?GetMazeInfo@CGocMaze@@QEAAPEAUTB_MAZE_INFO@@XZ
// 关联 IDA 0x1400857C0: XResourceMgr::GetTB_MAZE_INFO
TB_MAZE_INFO* CGocMaze::GetMazeInfo() const
{
    if (!m_pMaze)
    {
        return nullptr;
    }

    // 对齐 IDA: 从XMaze获取MapID，然后从资源管理器获取TB_MAZE_INFO
    // 参见 IDA XResourceMgr::GetTB_MAZE_INFO 实现 (0x1400857C0)
    // 查找 m_mapTB_MAZE_INFO map 表

    // TODO: 汇编还原 - 需要从XMaze实例获取MapID
    // std::uint16_t wMapID = m_pMaze->GetMapID();
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // return pServer->GetResourceMgr().GetTB_MAZE_INFO(wMapID);

    return nullptr;
}

// ============================================================================
// 迷宫进入/退出
// ============================================================================

// IDA: ?EnterMaze@CGocMaze@@QEAAHUXMapID@@@Z
int CGocMaze::EnterMaze(const UXMapID& uxMapID)
{
    // TODO: 汇编还原 - 需要从IDA反编译完整的进入迷宫逻辑
    // 返回: 0=成功, 错误码=失败

    // 推测逻辑：
    // 1. 检查是否已在迷宫中
    if (IsMaze())
    {
        return 55001;  // 错误码：已在迷宫中
    }

    // 2. 检查迷宫是否开放
    // 3. 检查进入条件（等级、任务、道具等）
    // 4. 创建或加入迷宫实例
    // 5. 更新统计

    m_nEnterCount++;

    return 0;  // 成功
}

// IDA: ?ExitMaze@CGocMaze@@QEAAXXZ
void CGocMaze::ExitMaze()
{
    // TODO: 汇编还原 - 需要从IDA反编译完整的退出迷宫逻辑

    if (!m_pMaze)
    {
        return;
    }

    // 推测逻辑：
    // 1. 从迷宫实例中移除玩家
    // 2. 清理迷宫状态
    // 3. 发送退出通知

    m_pMaze.reset();
}

// ============================================================================
// 迷宫完成/奖励
// ============================================================================

// IDA: ?ClearMaze@CGocMaze@@QEAAXH@Z
void CGocMaze::ClearMaze(int nClearType)
{
    // TODO: 汇编还原 - 需要从IDA反编译完整的完成迷宫逻辑
    // nClearType: 通关类型（正常通关、放弃、失败等）

    if (!m_pMaze)
    {
        return;
    }

    // 推测逻辑：
    // 1. 计算通关奖励
    // 2. 更新通关统计
    // 3. 记录通关时间
    // 4. 发送通关通知

    m_nClearCount++;
}

// IDA: ?GetReward@CGocMaze@@QEAA_NAEAV?$vector@UST_MAZE_REWARD@@V?$allocator@UST_MAZE_REWARD@@@std@@@std@@@Z
bool CGocMaze::GetReward(std::vector<ST_MAZE_REWARD>& vecRewards)
{
    // TODO: 汇编还原 - 需要从IDA反编译完整的奖励计算逻辑

    if (!m_pMaze)
    {
        return false;
    }

    vecRewards.clear();

    // 推测逻辑：
    // 1. 根据迷宫类型、难度、通关时间计算奖励
    // 2. 根据评价等级（F, C, B, A, S, SS）调整奖励
    // 3. 添加经验、金币、道具等奖励

    return true;
}

// ============================================================================
// 迷宫统计
// ============================================================================

// IDA: ?GetEnterCount@CGocMaze@@QEAAHXZ
int CGocMaze::GetEnterCount() const
{
    return m_nEnterCount;
}

// IDA: ?AddEnterCount@CGocMaze@@QEAAXH@Z
void CGocMaze::AddEnterCount(int nCount)
{
    m_nEnterCount += nCount;
}

// IDA: ?GetClearCount@CGocMaze@@QEAAHXZ
int CGocMaze::GetClearCount() const
{
    return m_nClearCount;
}

// IDA: ?AddClearCount@CGocMaze@@QEAAXH@Z
void CGocMaze::AddClearCount(int nCount)
{
    m_nClearCount += nCount;
}

// IDA: ?GetBestTime@CGocMaze@@QEAA_JXZ
std::int64_t CGocMaze::GetBestTime() const
{
    return m_biBestTime;
}

// IDA: ?SetBestTime@CGocMaze@@QEAAX_J@Z
void CGocMaze::SetBestTime(std::int64_t biTime)
{
    if (m_biBestTime == 0 || biTime < m_biBestTime)
    {
        m_biBestTime = biTime;
    }
}

// ============================================================================
// 迷宫记录
// ============================================================================

// IDA: ?GetMazeRecord@CGocMaze@@QEAAPEAUST_MAZE_RECORD@@H@Z
ST_MAZE_RECORD* CGocMaze::GetMazeRecord(int nIndex)
{
    // TODO: 汇编还原 - 需要实现迷宫记录存储
    // return &m_vecRecords[nIndex];
    return nullptr;
}

// IDA: ?SaveMazeRecord@CGocMaze@@QEAAXAEBUST_MAZE_RECORD@@@Z
void CGocMaze::SaveMazeRecord(const ST_MAZE_RECORD& stRecord)
{
    // TODO: 汇编还原 - 需要实现迷宫记录保存
    // m_vecRecords.push_back(stRecord);
}
