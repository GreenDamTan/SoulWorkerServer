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
class XArea;

// 对齐 IDA: CGameWorldMode 游戏世界模式类 (152 bytes / 0x98)
// 用于 GameServer 中的 WorldMode 管理
class CGameWorldMode {
public:
    CGameWorldMode();
    virtual ~CGameWorldMode();

    // 对齐 IDA: Init 初始化
    void Init(CBattleZone* pBattleZone, ST_WORLD_MODE_INFO* pstInfo);

    // IDA: ?Init@CGameWorldMode@@QEAAXPEAVXArea@@AEAUST_WORLD_MODE_INFO@@@Z (0x140722d80)
    void Init(XArea* pArea, ST_WORLD_MODE_INFO* pstInfo);

    // 对齐 IDA: StartMode 启动模式
    void StartMode();

    // 对齐 IDA: FinishMode 完成模式
    void FinishMode(PS_WORLD_MODE_FINISH* pstFinish);

    // 对齐 IDA: GetState 获取状态
    int GetState() const { return m_stInfo.nState; }

    // 对齐 IDA: GetModeID 获取模式ID
    int GetModeID() const { return m_stInfo.nModeID; }

    // IDA: ?GetModeDateID@CGameWorldMode@@QEAAHXZ (0x1401ad9c0)
    int GetModeDateID() const { return m_stInfo.nModeDateID; }

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

    // 对齐 IDA 0x1401AD970: IsSpawnNextMonster - 检查是否可以生成下一波怪物
    bool IsSpawnNextMonster() const {
        return !m_nMonsterClearCount || m_bNextMonster;
    }

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
    // 成员变量 (对齐 IDA CGameWorldMode)
    // IDA 0x140722CC0: vftable(8) + m_stInfo(56/0x38) + m_pArea(8) + ...

    // IDA: ST_WORLD_MODE_INFO m_stInfo - 内嵌结构体 (56 bytes / 0x38)
    ST_WORLD_MODE_INFO m_stInfo = {};       // 偏移 8, 大小 0x38

    // IDA: XArea* m_pArea
    XArea* m_pArea = nullptr;               // 偏移 0x40

    // IDA: std::list<int> m_listMonsterSpawnBox - 生成箱子列表
    std::list<int> m_listMonsterSpawnBox;   // 偏移 0x48

    // IDA: std::list<int> m_listTargetMonster - 目标怪物列表
    std::list<int> m_listTargetMonster;     // 偏移 0x60

    // IDA: std::list<int> m_listDropActorID - 掉落ActorID列表
    std::list<int> m_listDropActorID;       // 偏移 0x78

    // IDA: int m_nMonsterClearCount
    int m_nMonsterClearCount = 0;           // 偏移 0x90

    // IDA: int m_nMonserSummonCount (注意拼写错误是原始代码的)
    int m_nMonserSummonCount = 0;           // 偏移 0x94

    // IDA: bool m_bNextMonster
    bool m_bNextMonster = false;            // 偏移 0x98

    // 兼容性成员 (非 IDA，用于简化访问)
    CBattleZone* m_pBattleZone = nullptr;   // 用于 CBattleZone* 接口
    int m_nModeType = 0;                    // 模式类型
    std::list<CMonster*> m_listMonster;     // 怪物对象列表

    // 任务状态映射: QuestID -> Status (0=未开始, 1=进行中, 2=完成, 3=失败)
    std::unordered_map<int, int> m_mapQuestStatus;
};
