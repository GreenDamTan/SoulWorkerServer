// GocMaze.h
// CGocMaze - Game Object Component for Maze system
// 从 IDA GameServer.exe 精确还原

#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <cstdint>
#include <memory>
#include <vector>

// 前置声明
class CMover;
class CUser;
class XMaze;
struct TB_MAZE_INFO;
struct ST_MAZE_RECORD;

// ============================================================================
// CGocMaze - 迷宫组件类
// IDA 确认大小: 待验证
//
// 功能: 管理 CMover 的迷宫状态，作为 GOComponent 的子类附加到玩家对象上
// 管理:
// - 当前迷宫实例引用
// - 迷宫统计数据
// - 迷宫记录
// - 迷宫进入次数
// - 迷宫奖励相关
//
// IDA 内存布局 (推测):
// offset 0:  vtable (8 bytes from GOComponent)
// offset 8:  m_pOwner (8 bytes from GOComponent)
// offset 16: m_pMaze (std::shared_ptr<XMaze>, 16 bytes)
// offset 32: 迷宫统计数据
// ============================================================================
class CGocMaze : public GOComponent {
public:
    // === 构造函数 ===
    // IDA: ??0CGocMaze@@QEAA@XZ
    CGocMaze();

    // === 析构函数 ===
    // IDA: ??1CGocMaze@@UEAA@XZ
    virtual ~CGocMaze();

    // === 迷宫管理 ===

    // Init - 初始化迷宫组件
    // IDA: ?Init@CGocMaze@@QEAAXXZ
    void Init();

    // Clear - 清除迷宫状态
    // IDA: ?Clear@CGocMaze@@QEAAXXZ
    void Clear();

    // === 迷宫状态查询 ===

    // IsMaze - 检查是否在迷宫中
    // IDA: ?IsMaze@CGocMaze@@QEAA_NXZ
    bool IsMaze() const;

    // GetMaze - 获取迷宫实例
    // IDA: ?GetMaze@CGocMaze@@QEAAPEAVXMaze@@XZ
    XMaze* GetMaze() const;

    // GetMazeInfo - 获取迷宫信息表
    // IDA: ?GetMazeInfo@CGocMaze@@QEAAPEAUTB_MAZE_INFO@@XZ
    TB_MAZE_INFO* GetMazeInfo() const;

    // === 迷宫进入/退出 ===

    // EnterMaze - 进入迷宫
    // IDA: ?EnterMaze@CGocMaze@@QEAAHUXMapID@@@Z
    // 返回: 0=成功, 错误码=失败
    int EnterMaze(const UXMapID& uxMapID);

    // ExitMaze - 退出迷宫
    // IDA: ?ExitMaze@CGocMaze@@QEAAXXZ
    void ExitMaze();

    // === 迷宫完成/奖励 ===

    // ClearMaze - 完成迷宫
    // IDA: ?ClearMaze@CGocMaze@@QEAAXH@Z
    void ClearMaze(int nClearType);

    // GetReward - 获取迷宫奖励
    // IDA: ?GetReward@CGocMaze@@QEAA_NAEAV?$vector@UST_MAZE_REWARD@@V?$allocator@UST_MAZE_REWARD@@@std@@@std@@@Z
    bool GetReward(std::vector<ST_MAZE_REWARD>& vecRewards);

    // === 迷宫统计 ===

    // GetEnterCount - 获取迷宫进入次数
    // IDA: ?GetEnterCount@CGocMaze@@QEAAHXZ
    int GetEnterCount() const;

    // AddEnterCount - 增加进入次数
    // IDA: ?AddEnterCount@CGocMaze@@QEAAXH@Z
    void AddEnterCount(int nCount);

    // GetClearCount - 获取迷宫完成次数
    // IDA: ?GetClearCount@CGocMaze@@QEAAHXZ
    int GetClearCount() const;

    // AddClearCount - 增加完成次数
    // IDA: ?AddClearCount@CGocMaze@@QEAAXH@Z
    void AddClearCount(int nCount);

    // GetBestTime - 获取最佳通关时间
    // IDA: ?GetBestTime@CGocMaze@@QEAA_JXZ
    std::int64_t GetBestTime() const;

    // SetBestTime - 设置最佳通关时间
    // IDA: ?SetBestTime@CGocMaze@@QEAAX_J@Z
    void SetBestTime(std::int64_t biTime);

    // === 迷宫记录 ===

    // GetMazeRecord - 获取迷宫记录
    // IDA: ?GetMazeRecord@CGocMaze@@QEAAPEAUST_MAZE_RECORD@@H@Z
    ST_MAZE_RECORD* GetMazeRecord(int nIndex);

    // SaveMazeRecord - 保存迷宫记录
    // IDA: ?SaveMazeRecord@CGocMaze@@QEAAXAEBUST_MAZE_RECORD@@@Z
    void SaveMazeRecord(const ST_MAZE_RECORD& stRecord);

    // === 静态方法 ===

    // GetFamilyID - 获取家族ID (静态方法)
    // IDA: ?GetFamilyID@CGocMaze@@SAHXZ
    static int GetFamilyID();

protected:
    // === IDA 确认的成员变量 ===

    // 当前迷宫实例
    std::shared_ptr<XMaze> m_pMaze;

    // 迷宫进入次数
    int m_nEnterCount = 0;

    // 迷宫完成次数
    int m_nClearCount = 0;

    // 最佳通关时间
    std::int64_t m_biBestTime = 0;

    // TODO: 需要验证更多成员变量
    // 迷宫记录列表
    // std::vector<ST_MAZE_RECORD> m_vecRecords;
};

// 迷宫奖励结构
struct ST_MAZE_REWARD {
    std::uint32_t dwItemID = 0;
    std::uint32_t dwCount = 0;
    std::uint8_t byType = 0;  // 奖励类型
    std::uint8_t byGrade = 0; // 奖励等级
};

// 迷宫记录结构
struct ST_MAZE_RECORD {
    UXMapID uxMapID;
    std::int64_t biClearTime = 0;
    std::int32_t nScore = 0;
    std::int32_t nKillCount = 0;
    std::int32_t nDeathCount = 0;
    std::uint8_t byGrade = 0; // 评级
    std::uint8_t byClearType = 0; // 通关类型
};
