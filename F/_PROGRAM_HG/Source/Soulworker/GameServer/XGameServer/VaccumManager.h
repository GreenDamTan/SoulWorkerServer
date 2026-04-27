#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include <memory>

// 前置声明
class CBattleZone;
class CVaccumGroup;

// TODO: 推测结果 - 来自 IDA struct CVaccumManager (176 bytes)
class CVaccumManager {
public:
    CVaccumManager();
    ~CVaccumManager();

    void Init(CBattleZone* pArea, bool bAutoSpawn);
    void Clear();

    CBattleZone* GetArea() const { return m_pArea; }

    // 真空组管理
    bool AddVaccumGroup(int nID, std::tr1::shared_ptr<CVaccumGroup> pGroup);
    void RemoveVaccumGroup(int nID);
    CVaccumGroup* FindVaccumGroup(int nID);

    // 生成
    void SpawnAll();
    void OnUpdate(float fDelta);

private:
    // === IDA 确认的成员变量 ===
    // offset 0: m_mapVaccumGroup (std::map<int, shared_ptr<CVaccumGroup>>, 32 bytes)
    std::map<int, std::tr1::shared_ptr<void>> m_mapVaccumGroup;  // TODO: 需人工审查 - 类型待确认

    // offset 32: m_mapVaccumTableID (std::map<int, int>, 32 bytes)
    std::map<int, int> m_mapVaccumTableID;

    // offset 64: m_pArea (CBattleZone*, 8 bytes)
    CBattleZone* m_pArea;

    // offset 72: m_vecBoxID (std::vector<int>, 32 bytes)
    std::vector<int> m_vecBoxID;

    // offset 104: m_mapVaccumCheat (std::map<int, std::vector<int>>, 32 bytes)
    std::map<int, std::vector<int>> m_mapVaccumCheat;

    // offset 136: m_bAutoSpawn (bool)
    bool m_bAutoSpawn;

    // offset 144: m_mapVaccumNoneAuto (std::map<int, shared_ptr<CVaccumGroup>>, 32 bytes)
    std::map<int, std::tr1::shared_ptr<void>> m_mapVaccumNoneAuto;  // TODO: 需人工审查 - 类型待确认

    // Total size: 176 bytes (verified from IDA)
};
