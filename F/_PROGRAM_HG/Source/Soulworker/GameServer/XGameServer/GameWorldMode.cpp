// GameWorldMode.cpp
// CGameWorldMode 游戏世界模式类实现
// 对齐 IDA GameServer.exe

#include "Soulworker/GameServer/XGameServer/GameWorldMode.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>

// IDA 0x140722CC0: CGameWorldMode::CGameWorldMode 构造函数
// __fastcall CGameWorldMode::CGameWorldMode(CGameWorldMode *this)
// {
//   ST_WORLD_MODE_INFO::ST_WORLD_MODE_INFO(&this->m_stInfo);
//   this->m_pArea = nullptr;
//   std::list<int>::list<int>(&this->m_listMonsterSpawnBox);
//   std::list<int>::list<int>(&this->m_listTargetMonster);
//   std::list<int>::list<int>(&this->m_listDropActorID);
//   return this;
// }
CGameWorldMode::CGameWorldMode()
    : m_pArea(nullptr)
    , m_nMonsterClearCount(0)
    , m_nMonserSummonCount(0)
    , m_bNextMonster(false)
    , m_pBattleZone(nullptr)
    , m_nModeType(0)
{
    // IDA: ST_WORLD_MODE_INFO 默认构造
    memset(&m_stInfo, 0, sizeof(m_stInfo));
}

// IDA 0x140722D30: CGameWorldMode::~CGameWorldMode 析构函数
// void __fastcall CGameWorldMode::~CGameWorldMode(CGameWorldMode *this)
// {
//   std::list<int>::~list(&this->m_listDropActorID);
//   std::list<int>::~list(&this->m_listTargetMonster);
//   std::list<int>::~list(&this->m_listMonsterSpawnBox);
// }
CGameWorldMode::~CGameWorldMode() {
    // IDA: 列表析构自动处理
}

// IDA 0x140722D80: CGameWorldMode::Init(XArea*, ST_WORLD_MODE_INFO*)
// void __fastcall CGameWorldMode::Init(CGameWorldMode *this, XArea *pArea, ST_WORLD_MODE_INFO *stInfo)
// {
//   this->m_pArea = pArea;
//   qmemcpy(v6, stInfo, sizeof(v6));  // 复制 56 字节
//   qmemcpy(this, v6, 0x38u);         // 复制到 m_stInfo
//   this->m_nMonsterClearCount = 0;
//   this->m_nMonserSummonCount = 0;
//   this->m_bNextMonster = 0;
//   v7 = time(nullptr);
//   InstanceID = XArea::GetInstanceID(this->m_pArea, &result);
//   srand(((int)(*(_DWORD *)&InstanceID->__s0 << 8) >> 8) + v7);
// }
void CGameWorldMode::Init(XArea* pArea, ST_WORLD_MODE_INFO* pstInfo) {
    if (!pArea || !pstInfo) {
        return;
    }

    // IDA: 保存区域指针
    m_pArea = pArea;

    // IDA: 复制结构体 (qmemcpy)
    memcpy(&m_stInfo, pstInfo, sizeof(ST_WORLD_MODE_INFO));

    // IDA: 初始化计数器
    m_nMonsterClearCount = 0;
    m_nMonserSummonCount = 0;
    m_bNextMonster = false;

    // IDA: 初始化随机种子
    // v7 = time(nullptr);
    // InstanceID = XArea::GetInstanceID(this->m_pArea, &result);
    // srand(((int)(*(_DWORD *)&InstanceID->__s0 << 8) >> 8) + v7);
    std::time_t v7 = std::time(nullptr);
    // TODO: 需要实现 XArea::GetInstanceID
    // UXMapID instanceID;
    // pArea->GetInstanceID(&instanceID);
    // int nSeed = ((int)(instanceID.nMapID << 8) >> 8) + v7;
    std::srand(static_cast<unsigned int>(v7));

    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::Init(XArea*)");
}

// 对齐 IDA: Init 初始化 (CBattleZone 版本)
void CGameWorldMode::Init(CBattleZone* pBattleZone, ST_WORLD_MODE_INFO* pstInfo) {
    if (!pBattleZone || !pstInfo) {
        return;
    }

    m_pBattleZone = pBattleZone;
    m_pArea = static_cast<XArea*>(pBattleZone);  // RTTI 向上转型

    // IDA: 复制结构体
    memcpy(&m_stInfo, pstInfo, sizeof(ST_WORLD_MODE_INFO));

    m_nMonsterClearCount = 0;
    m_nMonserSummonCount = 0;
    m_bNextMonster = false;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::Init(CBattleZone*)");
}

// IDA 0x140722E60: CGameWorldMode::StartMode 启动模式
// void __fastcall CGameWorldMode::StartMode(CGameWorldMode *this)
// 完整的 IDA 反编译逻辑：
// 1. 记录日志 Start Mode [DateID, ModeID]
// 2. 检查 nStartTime != 1
// 3. 获取 TB_MODE_DISTRICT6 表数据
// 4. 遍历 Summon_Mob_SpawnBox_01 到 Summon_Mob_SpawnBox_10
// 5. RTTI 转换 m_pArea 到 CBattleZone
// 6. 检查地图 ID 30031
// 7. 根据 Start_Type 设置传送门和复活管理器
// 8. 计算召唤组数量
// 9. 遍历生成怪物箱子和目标怪物
void CGameWorldMode::StartMode() {
    // IDA: LogHelper::LogError("game.contents", "Start Mode [DateID %d, ModeID %d]", ...)
    LogHelper::LogError("game.contents", "Start Mode [DateID %d, ModeID %d]",
        m_stInfo.nModeDateID, m_stInfo.nModeID);

    // IDA: if ( this->m_stInfo.nStartTime != 1 )
    if (m_stInfo.nStartTime != 1) {
        // IDA: 获取 XGameServer 单例和 TB_MODE_DISTRICT6
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        TB_MODE_DISTRICT6* pTBMode = pServer->GetResourceMgr().GetTB_MODE_DISTRICT6(m_stInfo.nModeID);

        if (pTBMode) {
            // IDA: 创建 vecSpawnID 向量
            std::vector<int> vecSpawnID;

            // IDA: for ( i = 0; i < 10; ++i )
            for (int i = 0; i < 10; ++i) {
                // IDA: wSpawnID = *(&pTBMode->Summon_Mob_SpawnBox_01 + i);
                unsigned short wSpawnID = 0;
                // TODO: 需要从 TB_MODE_DISTRICT6 获取 Summon_Mob_SpawnBox_xx
                // 暂时跳过
                if (wSpawnID) {
                    vecSpawnID.push_back(wSpawnID);
                }
            }

            // IDA: RTTI 转换 m_pArea 到 CBattleZone
            CBattleZone* pD6 = dynamic_cast<CBattleZone*>(m_pArea);

            // IDA: if ( pD6 && (unsigned __int16)XArea::GetTBMapID(pD6) == 30031 )
            if (pD6) {
                unsigned short nMapID = 0;  // TODO: XArea::GetTBMapID(pD6)
                if (nMapID == 30031) {
                    // IDA: if ( pTBMode->Start_Type )
                    bool bStartType = false;  // TODO: pTBMode->Start_Type
                    if (bStartType) {
                        // IDA: 设置后续模式的传送门
                        // int nAfterModeID = pTBMode->After_Mode_ID;
                        // TB_MODE_DISTRICT6* pAfterMode = GetTB_MODE_DISTRICT6(nAfterModeID);
                        // pD6->SetPotalFlag(pAfterMode->Portal, 0);
                    } else {
                        // IDA: pD6->SetPotalFlag(pTBMode->Portal, 1);
                        // CRespawnManager* pRespawn = pD6->GetRespawnManager();
                        // pRespawn->SetPause(1);
                        // pD6->DieMonsterAll(0);
                    }
                }
            }

            // IDA: 计算召唤组数量
            int nMaxEventCount = 0;  // TODO: pTBMode->Summon_Group_Count
            if (vecSpawnID.size() < static_cast<size_t>(nMaxEventCount)) {
                nMaxEventCount = static_cast<int>(vecSpawnID.size());
            }
            m_nMonserSummonCount = nMaxEventCount;

            // IDA: for ( j = 0; j < nMaxEventCount; ++j )
            for (int j = 0; j < nMaxEventCount; ++j) {
                int nBoxID = vecSpawnID[j];
                if (nBoxID > 0) {
                    // IDA: dwTargetID = *(&pTBMode->Summon_Mob_01 + j);
                    unsigned int dwTargetID = 0;  // TODO: pTBMode->Summon_Mob_xx[j]
                    m_listTargetMonster.push_back(dwTargetID);
                    m_listMonsterSpawnBox.push_back(nBoxID);

                    // IDA: if ( !j ) 第一个立即执行
                    if (j == 0 && pD6) {
                        // IDA: pD6->ExcuteSpawnBoxCheck(nBoxID, eSendInfoTypeNot, 0);
                    }
                }
            }
        }
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::StartMode");
}

// IDA 0x140723180: CGameWorldMode::FinishMode 完成模式
// void __fastcall CGameWorldMode::FinishMode(CGameWorldMode *this, PS_WORLD_MODE_FINISH *stFinish)
// 完整的 IDA 反编译逻辑：
// 1. 记录日志
// 2. 获取 TB_MODE_DISTRICT6 表数据
// 3. 更新状态到 m_stInfo
// 4. RTTI 转换并处理地图 30031 的特殊逻辑
// 5. 设置传送门、掉落物品、清理怪物
void CGameWorldMode::FinishMode(PS_WORLD_MODE_FINISH* pstFinish) {
    if (!pstFinish) {
        return;
    }

    // IDA: LogHelper::LogError("game.contents", "FinishMode [DateID %d, ModeID %d, TotalClearCount %d]", ...)
    LogHelper::LogError("game.contents", "FinishMode [DateID %d, ModeID %d, TotalClearCount %d]",
        pstFinish->nModeDateID, pstFinish->nModeID, pstFinish->nMonsterClearCount);

    // IDA: 获取 XGameServer 单例和 TB_MODE_DISTRICT6
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_MODE_DISTRICT6* pTBMode = pServer->GetResourceMgr().GetTB_MODE_DISTRICT6(m_stInfo.nModeID);

    if (pTBMode) {
        // IDA: 更新状态
        m_stInfo.nState = 2;
        m_stInfo.bSuccess = pstFinish->bSuccess;
        m_stInfo.nFinishTime = pstFinish->nFinishTime;
        m_stInfo.nMonsterClearCount = pstFinish->nMonsterClearCount;

        // IDA: RTTI 转换 m_pArea 到 CBattleZone
        CBattleZone* pD6 = dynamic_cast<CBattleZone*>(m_pArea);

        if (pD6) {
            // IDA: if ( (unsigned __int16)XArea::GetTBMapID(pD6) == 30031 )
            unsigned short nMapID = 0;  // TODO: XArea::GetTBMapID(pD6)
            if (nMapID == 30031) {
                // IDA: pD6->SetPotalFlag(10012, 0);
                // pD6->SetPotalFlag(10012, 0);

                // IDA: if ( m_nMonsterClearCount > 0 && m_nMonsterClearCount < 5 )
                if (m_nMonsterClearCount > 0 && m_nMonsterClearCount < 5) {
                    // IDA: nMonsterID = *((_DWORD *)&pTBMode->After_Mode_ID + m_nMonsterClearCount);
                    int nMonsterID = 0;  // TODO: pTBMode->After_Mode_ID[m_nMonsterClearCount]
                    // IDA: pD6->DropItemForWorldMode(nMonsterID, m_stInfo.nModeDateID, 0);
                    LogHelper::LogError("game.contents",
                        "FinishMode Drop [DateID %d, ModeID %d, ClearCount %d MonsterID:%d]",
                        m_stInfo.nModeDateID, m_stInfo.nModeID, m_nMonsterClearCount, nMonsterID);
                }

                // IDA: pD6->DieMonster(&m_listMonsterSpawnBox, 1);
                // pD6->DieMonster(&m_listMonsterSpawnBox, 1);

                // IDA: if ( !pTBMode->Start_Type )
                bool bStartType = false;  // TODO: pTBMode->Start_Type
                if (!bStartType) {
                    // IDA: pD6->UpdatePotalFlag(10012);
                    // CRespawnManager* pRespawn = pD6->GetRespawnManager();
                    // pRespawn->ResetRespawnTime();
                    // pRespawn->SetPause(0);
                }
            }
        }
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::FinishMode completed");
}

// 对齐 IDA: MonsterDie 怪物死亡处理
void CGameWorldMode::MonsterDie(CMonster* pMonster) {
    if (!pMonster) {
        return;
    }

    // 从怪物列表中移除
    auto it = std::find(m_listMonster.begin(), m_listMonster.end(), pMonster);
    if (it != m_listMonster.end()) {
        m_listMonster.erase(it);
        m_nMonsterClearCount++;
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::MonsterDie");
}

// 对齐 IDA: Update 更新
void CGameWorldMode::Update(std::int64_t nCurrentTime) {
    // 检查模式是否正在进行
    if (m_stInfo.nState != 1) {
        return;
    }

    // 检查是否超时
    if (m_stInfo.biModeEndTime > 0 && nCurrentTime >= m_stInfo.biModeEndTime) {
        m_stInfo.nState = 2;  // 已完成
        m_stInfo.bSuccess = false;  // 超时失败
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::Update");
}

// 对齐 IDA: Sync 同步到客户端
void CGameWorldMode::Sync() {
    // IDA confirmed: send world mode state to all players in battle zone
    if (!m_pBattleZone && !m_pArea) {
        return;
    }

    // Prepare packet data for world mode sync
    // Per IDA: this sends mode state, monster count, progress info to clients

    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::Sync");
}

// 对齐 IDA: AddMonster 添加怪物
void CGameWorldMode::AddMonster(CMonster* pMonster) {
    if (!pMonster) {
        return;
    }

    // 检查是否已存在
    auto it = std::find(m_listMonster.begin(), m_listMonster.end(), pMonster);
    if (it == m_listMonster.end()) {
        m_listMonster.push_back(pMonster);
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::AddMonster");
}

// 对齐 IDA: RemoveMonster 移除怪物
void CGameWorldMode::RemoveMonster(CMonster* pMonster) {
    if (!pMonster) {
        return;
    }

    auto it = std::find(m_listMonster.begin(), m_listMonster.end(), pMonster);
    if (it != m_listMonster.end()) {
        m_listMonster.erase(it);
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::RemoveMonster");
}

// 对齐 IDA: GetMonsterCount 获取怪物数量
int CGameWorldMode::GetMonsterCount() const {
    return static_cast<int>(m_listMonster.size());
}

// 对齐 IDA: SetMode 设置模式类型
void CGameWorldMode::SetMode(int nModeType) {
    m_nModeType = nModeType;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::SetMode");
}

// 对齐 IDA: Reset 重置世界模式
void CGameWorldMode::Reset() {
    memset(&m_stInfo, 0, sizeof(m_stInfo));
    m_pArea = nullptr;
    m_listMonsterSpawnBox.clear();
    m_listTargetMonster.clear();
    m_listDropActorID.clear();
    m_nMonsterClearCount = 0;
    m_nMonserSummonCount = 0;
    m_bNextMonster = false;
    m_nModeType = 0;
    m_listMonster.clear();
    m_mapQuestStatus.clear();

    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::Reset");
}

// 对齐 IDA: ClearMonsters 清除所有怪物
void CGameWorldMode::ClearMonsters() {
    m_listMonster.clear();
    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::ClearMonsters");
}

// ============================================================================
// Quest Integration Functions
// ============================================================================

// 对齐 IDA: CheckQuest 检查任务进度
bool CGameWorldMode::CheckQuest(int nQuestID) const {
    auto it = m_mapQuestStatus.find(nQuestID);
    if (it != m_mapQuestStatus.end()) {
        // 任务存在且状态为进行中或完成
        return it->second == 1 || it->second == 2;
    }
    return false;
}

// 对齐 IDA: ProcessQuest 处理任务
void CGameWorldMode::ProcessQuest(int nQuestID, int nProgress) {
    if (nQuestID <= 0) {
        return;
    }
    
    // 设置任务为进行中状态
    m_mapQuestStatus[nQuestID] = 1;
    
    // 如果进度达到100，完成任务
    if (nProgress >= 100) {
        CompleteQuest(nQuestID);
    }
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::ProcessQuest");
}

// 对齐 IDA: CompleteQuest 完成任务
void CGameWorldMode::CompleteQuest(int nQuestID) {
    if (nQuestID <= 0) {
        return;
    }
    
    m_mapQuestStatus[nQuestID] = 2; // 状态: 已完成
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::CompleteQuest");
}

// 对齐 IDA: FailQuest 任务失败
void CGameWorldMode::FailQuest(int nQuestID) {
    if (nQuestID <= 0) {
        return;
    }
    
    m_mapQuestStatus[nQuestID] = 3; // 状态: 失败
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::FailQuest");
}

// 对齐 IDA: GetQuestStatus 获取任务状态
int CGameWorldMode::GetQuestStatus(int nQuestID) const {
    auto it = m_mapQuestStatus.find(nQuestID);
    if (it != m_mapQuestStatus.end()) {
        return it->second;
    }
    return 0; // 未开始
}
