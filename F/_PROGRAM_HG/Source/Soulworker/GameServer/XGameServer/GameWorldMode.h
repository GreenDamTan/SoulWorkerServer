// GameWorldMode.h
// CGameWorldMode 游戏世界模式类定义
// 对齐 IDA GameServer.exe

#pragma once

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <cstdint>
#include <memory>
#include <list>
#include <vector>
#include <unordered_map>

class CBattleZone;
class CMonster;

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

    // 对齐 IDA: MonsterDie 怪物死亡处理
    void MonsterDie(CMonster* pMonster);

    // 对齐 IDA: Update 更新
    void Update(std::int64_t nCurrentTime);

    // 对齐 IDA: Sync 同步到客户端
    void Sync();

    // 对齐 IDA: AddMonster 添加怪物
    void AddMonster(CMonster* pMonster);

    // 对齐 IDA: RemoveMonster 移除怪物
    void RemoveMonster(CMonster* pMonster);

    // 对齐 IDA: GetMonsterCount 获取怪物数量
    int GetMonsterCount() const;

    // 对齐 IDA: GetMonsterList 获取怪物列表
    const std::list<CMonster*>& GetMonsterList() const { return m_listMonster; }

    // 对齐 IDA: SetMode 设置模式类型
    void SetMode(int nModeType);

    // 对齐 IDA: GetMode 获取模式类型
    int GetMode() const { return m_nModeType; }

    // 对齐 IDA: Reset 重置世界模式
    void Reset();
    
    // 对齐 IDA: ClearMonsters 清除所有怪物
    void ClearMonsters();
    
    // === Quest Integration Functions ===
    
    // CheckQuest - 检查任务进度
    bool CheckQuest(int nQuestID) const;
    
    // ProcessQuest - 处理任务
    void ProcessQuest(int nQuestID, int nProgress);
    
    // CompleteQuest - 完成任务
    void CompleteQuest(int nQuestID);
    
    // FailQuest - 任务失败
    void FailQuest(int nQuestID);
    
    // GetQuestStatus - 获取任务状态
    int GetQuestStatus(int nQuestID) const;

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
    int m_nModeType = 0;                // 模式类型
    std::list<CMonster*> m_listMonster; // 怪物列表
    
    // 任务状态映射: QuestID -> Status (0=未开始, 1=进行中, 2=完成, 3=失败)
    std::unordered_map<int, int> m_mapQuestStatus;
};
