#include "Soulworker/GameServer/XGameServer/RespawnManager.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"
#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XGameServer/WorldManager.h"
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <ctime>
#endif

// 获取当前时间戳 (毫秒)
static std::uint64_t GreenDamTan_GetTickCount64() {
#ifdef _WIN32
    return GetTickCount64();
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (std::uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
}

// Per IDA 0x14063E3F0: CRespawnManager 构造函数
CRespawnManager::CRespawnManager()
    : m_bPause(false)
{
    // maps are default-constructed
}

// Per IDA 0x1401ADDC0: CRespawnManager 析构函数
CRespawnManager::~CRespawnManager() {
    // maps are automatically destroyed
}

// Per IDA 0x14063E440: Clear
void CRespawnManager::Clear() {
    m_mapRespawnWaitObject.clear();
    m_mapRespawnObject.clear();
    m_bPause = false;
}

// Per IDA 0x1401CA220: SetPause
void CRespawnManager::SetPause(bool bPause) {
    m_bPause = bPause;
}

// Per IDA 0x14063E480: RegisterMonster
// 注册普通怪物到重生管理器（暂不设置重生时间）
void CRespawnManager::RegisterMonster(
    std::uint32_t dwActor,
    int nTableID,
    int nObjectType,
    const VMonsterSpawnInfo* pMonsterSpawn)
{
    if (!pMonsterSpawn) {
        return;
    }

    ST_RESPAWN_OBJECT stObject;
    stObject.nTableID = nTableID;
    stObject.nObjectType = nObjectType;
    stObject.pSpawnInfo = pMonsterSpawn;
    stObject.nConditionID = 0;
    stObject.dwNextRespawnTime = 0;

    m_mapRespawnObject[dwActor] = stObject;
}

// Per IDA 0x14063E510: RegisterQuestMonster
// 注册任务怪物到重生管理器，并设置重生时间
void CRespawnManager::RegisterQuestMonster(
    std::uint32_t dwActor,
    int nTableID,
    int nObjectType,
    int nConditionID,
    const VMonsterSpawnInfo* pMonsterSpawn)
{
    if (!pMonsterSpawn) {
        return;
    }

    ST_RESPAWN_OBJECT stObject;
    stObject.nTableID = nTableID;
    stObject.nObjectType = nObjectType;
    stObject.pSpawnInfo = pMonsterSpawn;
    stObject.nConditionID = nConditionID;

    // Per IDA: 计算下次重生时间 = 当前时间 + 重生延迟
    // GetTickCount64 返回毫秒，m_RespawnTime 是秒（浮点）
    std::uint64_t dwCurrentTime = GreenDamTan_GetTickCount64();
    float fRespawnTimeMs = pMonsterSpawn->m_RespawnTime * 1000.0f;
    stObject.dwNextRespawnTime = dwCurrentTime + static_cast<std::uint64_t>(fRespawnTimeMs);

    m_mapRespawnWaitObject[dwActor] = stObject;
}

// Per IDA 0x14063E610: RemoveQuestMonster
// 移除所有指定任务条件的重生对象
void CRespawnManager::RemoveQuestMonster(
    int nConditionID,
    std::vector<const VMonsterSpawnInfo*>& vecDeleteBox)
{
    // 遍历等待重生的对象 (m_mapRespawnWaitObject)
    auto it_wait = m_mapRespawnWaitObject.begin();
    while (it_wait != m_mapRespawnWaitObject.end()) {
        if (it_wait->second.nConditionID == nConditionID) {
            // 添加到删除列表
            vecDeleteBox.push_back(it_wait->second.pSpawnInfo);
            // 从 map 中移除
            it_wait = m_mapRespawnWaitObject.erase(it_wait);
        } else {
            ++it_wait;
        }
    }

    // 遍历活跃的重生对象 (m_mapRespawnObject)
    auto it = m_mapRespawnObject.begin();
    while (it != m_mapRespawnObject.end()) {
        if (it->second.nConditionID == nConditionID) {
            // 添加到删除列表
            vecDeleteBox.push_back(it->second.pSpawnInfo);
            // 从 map 中移除
            it = m_mapRespawnObject.erase(it);
        } else {
            ++it;
        }
    }
}

// Per IDA 0x14063E810: DieRespawnMonster
// 怪物死亡时设置重生时间
void CRespawnManager::DieRespawnMonster(std::uint32_t dwActorID) {
    auto it = m_mapRespawnObject.find(dwActorID);
    if (it == m_mapRespawnObject.end()) {
        return;
    }

    // 获取重生延迟时间
    const VMonsterSpawnInfo* pSpawnInfo = it->second.pSpawnInfo;
    if (!pSpawnInfo) {
        return;
    }

    // Per IDA: 计算下次重生时间 = 当前时间 + 重生延迟
    std::uint64_t dwCurrentTime = GreenDamTan_GetTickCount64();
    float fRespawnTimeMs = pSpawnInfo->m_RespawnTime * 1000.0f;
    it->second.dwNextRespawnTime = dwCurrentTime + static_cast<std::uint64_t>(fRespawnTimeMs);

    // Per IDA: 将对象从活跃 map 移动到等待 map
    m_mapRespawnWaitObject[dwActorID] = it->second;
    m_mapRespawnObject.erase(it);
}

// Per IDA 0x14063E910: ResetRespawnTime
// 重置所有重生对象的重生时间（添加随机偏移）
void CRespawnManager::ResetRespawnTime() {
    XWorldManager* pWorldManager = XWorldManager::Instance();

    for (auto it = m_mapRespawnObject.begin(); it != m_mapRespawnObject.end(); ++it) {
        const VMonsterSpawnInfo* pSpawnInfo = it->second.pSpawnInfo;
        if (!pSpawnInfo) {
            continue;
        }

        // Per IDA: 使用 XWorldManager::nRand 生成 1-300 秒随机偏移
        int nRandSec = pWorldManager->nRand(1, 300);

        // Per IDA: 计算新的重生时间
        std::uint64_t dwCurrentTime = GreenDamTan_GetTickCount64();
        float fRespawnTimeMs = (pSpawnInfo->m_RespawnTime + static_cast<float>(nRandSec)) * 1000.0f;
        it->second.dwNextRespawnTime = dwCurrentTime + static_cast<std::uint64_t>(fRespawnTimeMs);
    }
}

// Per IDA 0x14063EA50: Update
// 更新重生逻辑
void CRespawnManager::Update(XArea* pArea) {
    if (!pArea) {
        return;
    }

    // Per IDA: 检查世界类型（非迷宫/战场则跳过）
    int nWorldType = pArea->GetWorldType();
    if (nWorldType == 0 || m_bPause) {
        return;
    }

    // Per IDA: 将等待重生中的对象移动到活跃 map
    for (auto it_wait = m_mapRespawnWaitObject.begin(); it_wait != m_mapRespawnWaitObject.end(); ) {
        m_mapRespawnObject[it_wait->first] = it_wait->second;
        it_wait = m_mapRespawnWaitObject.erase(it_wait);
    }

    // Per IDA: 检查活跃对象的重生时间
    std::uint64_t dwCurrentTime = GreenDamTan_GetTickCount64();

    for (auto it = m_mapRespawnObject.begin(); it != m_mapRespawnObject.end(); ) {
        ST_RESPAWN_OBJECT& object = it->second;

        // Per IDA: 检查是否到达重生时间
        if (object.dwNextRespawnTime != 0 && object.dwNextRespawnTime <= dwCurrentTime) {
            // Per IDA: 根据世界类型执行重生
            // nWorldType 1 = XMaze, 其他 = CBattleZone
            // TODO: XMaze type handling when XMaze is fully defined
            // if (nWorldType == 1) {
            //     XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
            //     if (pMaze) {
            //         pMaze->ExcuteSpawn(object.nTableID, object.nObjectType, object.pSpawnInfo, eSendInfoTypeSend);
            //     }
            // } else {
                // BattleZone 类型
                CBattleZone* pBattleZone = dynamic_cast<CBattleZone*>(pArea);
                if (pBattleZone) {
                    // TODO: ExcuteSpawn when properly defined
                    // pBattleZone->ExcuteSpawn(object.nTableID, object.nObjectType, object.pSpawnInfo, eSendInfoTypeSend);
                }
            // }

            // 重生后从 map 中移除
            it = m_mapRespawnObject.erase(it);
        } else {
            ++it;
        }
    }
}

// ============================================================================
// 辅助函数 (Round 6 Phase 5)
// ============================================================================

// Add - Add respawn entry (wrapper for RegisterMonster)
bool CRespawnManager::Add(std::uint32_t dwActor, int nTableID, int nObjectType, const VMonsterSpawnInfo* pSpawnInfo) {
    if (!pSpawnInfo) {
        return false;
    }

    RegisterMonster(dwActor, nTableID, nObjectType, pSpawnInfo);
    return true;
}

// Remove - Remove respawn entry
bool CRespawnManager::Remove(std::uint32_t dwActor) {
    auto itWait = m_mapRespawnWaitObject.find(dwActor);
    if (itWait != m_mapRespawnWaitObject.end()) {
        m_mapRespawnWaitObject.erase(itWait);
        return true;
    }

    auto it = m_mapRespawnObject.find(dwActor);
    if (it != m_mapRespawnObject.end()) {
        m_mapRespawnObject.erase(it);
        return true;
    }

    return false;
}

// Process - Process respawns (wrapper for Update)
void CRespawnManager::Process(XArea* pArea) {
    Update(pArea);
}

// GetCount - Get entry count
int CRespawnManager::GetCount() const {
    return static_cast<int>(m_mapRespawnObject.size() + m_mapRespawnWaitObject.size());
}

// GetNextRespawn - Get next respawn time
std::uint64_t CRespawnManager::GetNextRespawn(std::uint32_t dwActor) const {
    auto itWait = m_mapRespawnWaitObject.find(dwActor);
    if (itWait != m_mapRespawnWaitObject.end()) {
        return itWait->second.dwNextRespawnTime;
    }

    auto it = m_mapRespawnObject.find(dwActor);
    if (it != m_mapRespawnObject.end()) {
        return it->second.dwNextRespawnTime;
    }

    return 0;
}

// CancelRespawn - Cancel respawn
bool CRespawnManager::CancelRespawn(std::uint32_t dwActor) {
    return Remove(dwActor);
}

// Pause - Pause respawns
void CRespawnManager::Pause() {
    SetPause(true);
}

// Resume - Resume respawns
void CRespawnManager::Resume() {
    SetPause(false);
}

// GetList - Get respawn list
void CRespawnManager::GetList(std::vector<std::uint32_t>& vecList) const {
    vecList.clear();

    for (const auto& pair : m_mapRespawnWaitObject) {
        vecList.push_back(pair.first);
    }

    for (const auto& pair : m_mapRespawnObject) {
        vecList.push_back(pair.first);
    }
}
