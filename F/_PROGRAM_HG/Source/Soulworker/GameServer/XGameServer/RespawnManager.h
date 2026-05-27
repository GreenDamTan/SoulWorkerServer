#pragma once

#include <cstdint>
#include <map>
#include <vector>

// 前置声明
struct VMonsterSpawnInfo;
class XArea;

// Per IDA: ST_RESPAWN_OBJECT (32 bytes)
struct ST_RESPAWN_OBJECT {
    int nTableID;                              // offset 0
    int nObjectType;                           // offset 4
    int nConditionID;                          // offset 8
    // padding at offset 12 (4 bytes)
    const VMonsterSpawnInfo* pSpawnInfo;       // offset 16
    std::uint64_t dwNextRespawnTime;           // offset 24
};

// Per IDA: CRespawnManager (72 bytes)
class CRespawnManager {
public:
    CRespawnManager();
    ~CRespawnManager();

    // 清空所有重生对象
    void Clear();

    // 暂停/恢复重生
    void SetPause(bool bPause);
    bool IsPaused() const { return m_bPause; }

    // 注册怪物到重生管理器
    void RegisterMonster(std::uint32_t dwActor, int nTableID, int nObjectType, const VMonsterSpawnInfo* pMonsterSpawn);

    // 注册任务怪物到重生管理器
    void RegisterQuestMonster(std::uint32_t dwActor, int nTableID, int nObjectType, int nConditionID, const VMonsterSpawnInfo* pMonsterSpawn);

    // 移除任务怪物
    void RemoveQuestMonster(int nConditionID, std::vector<const VMonsterSpawnInfo*>& vecDeleteBox);

    // 怪物死亡时处理重生
    void DieRespawnMonster(std::uint32_t dwActorID);

    // 重置重生时间
    void ResetRespawnTime();

    // 更新重生逻辑
    void Update(XArea* pArea);

    // 辅助函数 (Round 6 Phase 5)
    bool Add(std::uint32_t dwActor, int nTableID, int nObjectType, const VMonsterSpawnInfo* pSpawnInfo);
    bool Remove(std::uint32_t dwActor);
    void Process(XArea* pArea);
    int GetCount() const;
    std::uint64_t GetNextRespawn(std::uint32_t dwActor) const;
    bool CancelRespawn(std::uint32_t dwActor);
    void Pause();
    void Resume();
    void GetList(std::vector<std::uint32_t>& vecList) const;

private:
    // === IDA 确认的成员变量 ===
    // offset 0: m_mapRespawnWaitObject (std::map<int, ST_RESPAWN_OBJECT>, 32 bytes)
    // 等待重生的对象（已死亡，等待重生时间）
    std::map<int, ST_RESPAWN_OBJECT> m_mapRespawnWaitObject;

    // offset 32: m_mapRespawnObject (std::map<int, ST_RESPAWN_OBJECT>, 32 bytes)
    // 活跃的重生对象（已重生，死亡后移动到 Wait）
    std::map<int, ST_RESPAWN_OBJECT> m_mapRespawnObject;

    // offset 64: m_bPause (bool)
    bool m_bPause;

    // Total size: 72 bytes (verified from IDA)
};
