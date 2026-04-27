#pragma once

#include <cstdint>
#include <map>

// 前置声明
struct ST_RESPAWN_OBJECT;

// TODO: 推测结果 - 来自 IDA struct CRespawnManager (72 bytes)
class CRespawnManager {
public:
    CRespawnManager();
    ~CRespawnManager();

    void Clear();
    void Pause(bool bPause);
    bool IsPaused() const { return m_bPause; }

    // 添加/删除重生对象
    bool AddRespawnObject(int nID, ST_RESPAWN_OBJECT& stObject);
    void RemoveRespawnObject(int nID);
    ST_RESPAWN_OBJECT* FindRespawnObject(int nID);

    void OnUpdate(float fDelta);

private:
    // === IDA 确认的成员变量 ===
    // offset 0: m_mapRespawnWaitObject (std::map<int, ST_RESPAWN_OBJECT>, 32 bytes)
    std::map<int, void*> m_mapRespawnWaitObject;  // TODO: 需人工审查 - 类型待确认

    // offset 32: m_mapRespawnObject (std::map<int, ST_RESPAWN_OBJECT>, 32 bytes)
    std::map<int, void*> m_mapRespawnObject;  // TODO: 需人工审查 - 类型待确认

    // offset 64: m_bPause (bool)
    bool m_bPause;

    // Total size: 72 bytes (verified from IDA)
};
