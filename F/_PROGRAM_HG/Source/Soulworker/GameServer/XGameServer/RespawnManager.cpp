#include "Soulworker/GameServer/XGameServer/RespawnManager.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"
#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include <vector>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#else
#include <ctime>
#endif

// 前置声明 XMaze (待完整实现)
class XMaze;

// eSendInfoTypeSend 枚举值 (来自 BattleZone.h)
enum E_SEND_INFO_TYPE_LOCAL {
    eSendInfoTypeNone = 0,
    eSendInfoTypeAll = 1,
    eSendInfoTypeNearby = 2,
    eSendInfoTypeSelf = 3,
    eSendInfoTypeSend = 4,
};

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

    // 计算下次重生时间 = 当前时间 + 重生延迟
    std::uint64_t dwCurrentTime = GreenDamTan_GetTickCount64();
    float fRespawnTime = pMonsterSpawn->m_RespawnTime; // in seconds, stored as float
    stObject.dwNextRespawnTime = dwCurrentTime + static_cast<std::uint64_t>(fRespawnTime * 1000.0f);

    m_mapRespawnWaitObject[dwActor] = stObject;
}

// Per IDA 0x14063E610: RemoveQuestMonster
// 移除所有指定任务条件的重生对象
void CRespawnManager::RemoveQuestMonster(
    int nConditionID,
    std::vector<const VMonsterSpawnInfo*>& vecDeleteBox)
{
    // 遍历等待重生的对象
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

    // 遍历活跃的重生对象
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

    // 计算下次重生时间 = 当前时间 + 重生延迟
    std::uint64_t dwCurrentTime = GreenDamTan_GetTickCount64();
    float fRespawnTime = pSpawnInfo->m_RespawnTime;
    it->second.dwNextRespawnTime = dwCurrentTime + static_cast<std::uint64_t>(fRespawnTime * 1000.0f);

    // 将对象从活跃 map 移动到等待 map
    m_mapRespawnWaitObject[dwActorID] = it->second;
    m_mapRespawnObject.erase(it);
}

// Per IDA 0x14063E910: ResetRespawnTime
// 重置所有重生对象的重生时间（添加随机偏移）
void CRespawnManager::ResetRespawnTime() {
    for (auto it = m_mapRespawnObject.begin(); it != m_mapRespawnObject.end(); ++it) {
        const VMonsterSpawnInfo* pSpawnInfo = it->second.pSpawnInfo;
        if (!pSpawnInfo) {
            continue;
        }

        // 随机 1-300 秒偏移
        int nRandSec = (rand() % 300) + 1;

        // 计算新的重生时间
        std::uint64_t dwCurrentTime = GreenDamTan_GetTickCount64();
        float fRespawnTime = pSpawnInfo->m_RespawnTime;
        it->second.dwNextRespawnTime = dwCurrentTime + static_cast<std::uint64_t>((fRespawnTime + static_cast<float>(nRandSec)) * 1000.0f);
    }
}

// Per IDA 0x14063EA50: Update
// 更新重生逻辑
void CRespawnManager::Update(XArea* pArea) {
    if (!pArea) {
        return;
    }

    // 检查世界类型（非迷宫/战场则跳过）
    int nWorldType = pArea->GetWorldType();
    if (nWorldType == 0 || m_bPause) {
        return;
    }

    // 将等待重生中的对象移动到活跃 map（它们已经准备好重生了）
    // 注意：IDA 显示这里是从 m_mapRespawnWaitObject 移动到 m_mapRespawnObject
    // 但实际逻辑似乎是：等待时间到了的对象需要重新生成
    // 这里先清空等待队列，将其加入活跃队列
    for (auto it_wait = m_mapRespawnWaitObject.begin(); it_wait != m_mapRespawnWaitObject.end(); ) {
        m_mapRespawnObject[it_wait->first] = it_wait->second;
        it_wait = m_mapRespawnWaitObject.erase(it_wait);
    }

    // 检查活跃对象的重生时间
    std::uint64_t dwCurrentTime = GreenDamTan_GetTickCount64();

    for (auto it = m_mapRespawnObject.begin(); it != m_mapRespawnObject.end(); ) {
        ST_RESPAWN_OBJECT& object = it->second;

        // 检查是否到达重生时间
        if (object.dwNextRespawnTime != 0 && object.dwNextRespawnTime <= dwCurrentTime) {
            // 根据世界类型执行重生
            // Per IDA: nWorldType 1 = XMaze, 2 = CBattleZone 等
            // ExcuteSpawn 参数: (int nBoxIndex, int nSpawnIndex, const VMonsterSpawnInfo* pSpawnInfo, E_SEND_INFO_TYPE eSendType)
            // nTableID 对应 nBoxIndex, nObjectType 对应 nSpawnIndex

            if (nWorldType == 1) {
                // Maze 类型 - XMaze 未完整定义，暂时使用 ExcuteSpawnBox
                // TODO: 当 XMaze 完整实现后，使用 XMaze::ExcuteSpawn
                CBattleZone* pBattleZone = dynamic_cast<CBattleZone*>(pArea);
                if (pBattleZone) {
                    pBattleZone->ExcuteSpawn(object.nTableID, object.nObjectType, object.pSpawnInfo, static_cast<E_SEND_INFO_TYPE>(eSendInfoTypeSend));
                }
            } else if (nWorldType == 2) {
                // BattleZone 类型
                CBattleZone* pBattleZone = dynamic_cast<CBattleZone*>(pArea);
                if (pBattleZone) {
                    pBattleZone->ExcuteSpawn(object.nTableID, object.nObjectType, object.pSpawnInfo, static_cast<E_SEND_INFO_TYPE>(eSendInfoTypeSend));
                }
            }

            // 重生后从 map 中移除
            it = m_mapRespawnObject.erase(it);
        } else {
            ++it;
        }
    }
}
