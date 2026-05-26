// GameWorldMode.h
// CGameWorldMode 游戏世界模式类定义
// 对齐 IDA GameServer.exe

#pragma once

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <cstdint>
#include <memory>

class CBattleZone;

// 对齐 IDA: CGameWorldMode 游戏世界模式类 (152 bytes / 0x98)
// 用于 GameServer 中的 WorldMode 管理
class CGameWorldMode {
public:
    CGameWorldMode();
    virtual ~CGameWorldMode();

    // 对齐 IDA: Init 初始化
    void Init(CBattleZone* pBattleZone, ST_WORLD_MODE_INFO* pstInfo);

    // 对齐 IDA: StartMode 启动模式
    void StartMode();

    // 对齐 IDA: FinishMode 完成模式
    void FinishMode(PS_WORLD_MODE_FINISH* pstFinish);

    // 对齐 IDA: GetState 获取状态
    int GetState() const { return m_nState; }

    // 对齐 IDA: GetModeID 获取模式ID
    int GetModeID() const { return m_nModeID; }

private:
    // 成员变量 (对齐 IDA CGameWorldMode - 152 bytes / 0x98)
    // vftable(8) + m_nModeID(4) + m_nModeDateID(4) + m_nState(4) + ...
    int m_nModeID = 0;                  // 模式ID
    int m_nModeDateID = 0;              // 模式日期ID
    int m_nState = 0;                   // 状态 (0=未开始, 1=进行中, 2=已完成)
    std::int64_t m_nStartTime = 0;      // 开始时间
    std::int64_t m_nFinishTime = 0;     // 结束时间
    std::int64_t m_biModeStartTime = 0; // 模式开始时间
    std::int64_t m_biModeEndTime = 0;   // 模式结束时间
    CBattleZone* m_pBattleZone = nullptr; // 所属战斗区域
    int m_nMonsterClearCount = 0;       // 怪物清除计数
    bool m_bSuccess = false;            // 是否成功完成
};
