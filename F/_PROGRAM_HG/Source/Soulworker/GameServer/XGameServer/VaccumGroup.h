#pragma once

#include <cstdint>
#include <map>
#include <queue>
#include <memory>

// 前置声明
class CVaccumManager;
class CVaccumCube;
class XActor;
union UXActorID;
struct VInterActionBoxInfo;
struct TB_INTERACTION_OBJECT;

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_StdCompat.h"

// 优先级比较器 (用于 priority_queue)
struct Prioritize {
    bool operator()(const CVaccumCube* a, const CVaccumCube* b) const;
};

// Per IDA: CVaccumGroup - 104 bytes
class CVaccumGroup {
public:
    CVaccumGroup(CVaccumManager* pManager, bool bAutoSpawn);
    ~CVaccumGroup();

    // Per IDA 0x140191840: 添加真空立方体
    bool AddVaccumCube(UXActorID uxActor, VInterActionBoxInfo* pInfo);

    // Per IDA 0x140191a90: 获取随机值
    int GetRandomValue();

    // Per IDA 0x140191ac0: 更新
    void Update();

    // Per IDA 0x140191f60: 点击
    unsigned int Click(int nID, XActor* pActor);

    // Per IDA 0x1401920b0: 取消点击
    unsigned int CancelClick(int nID, XActor* pActor);

    // Per IDA 0x140192f10: 激活真空立方体
    void ActiveVaccumCube();

    int GetFirstVaccumCubeID() const;

private:
    // === IDA 确认的成员变量 ===
    // offset 0: m_queueNonActiveVaccumCube (priority_queue<CVaccumCube*>)
    std::priority_queue<CVaccumCube*, std::vector<CVaccumCube*>, Prioritize> m_queueNonActiveVaccumCube;

    // offset 24: m_mapActiveVaccumCube (std::map<int, CVaccumCube*>)
    std::map<int, CVaccumCube*> m_mapActiveVaccumCube;

    // offset 56: m_pVaccumManager (CVaccumManager*)
    CVaccumManager* m_pVaccumManager;

    // offset 64: m_pTBInteraction (TB_INTERACTION_OBJECT*)
    TB_INTERACTION_OBJECT* m_pTBInteraction;

    // offset 72: m_dwNextSpawnTime (unsigned __int64)
    std::uint64_t m_dwNextSpawnTime;

    // offset 80: m_bAutoSpawn (bool)
    bool m_bAutoSpawn;

    // Total size: 104 bytes (estimated)
};
