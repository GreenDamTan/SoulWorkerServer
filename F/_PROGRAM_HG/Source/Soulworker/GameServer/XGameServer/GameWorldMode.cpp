// GameWorldMode.cpp
// CGameWorldMode 游戏世界模式类实现
// 对齐 IDA GameServer.exe

#include "Soulworker/GameServer/XGameServer/GameWorldMode.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <algorithm>

// 对齐 IDA: CGameWorldMode 构造函数
CGameWorldMode::CGameWorldMode()
    : m_nModeID(0)
    , m_nModeDateID(0)
    , m_nState(0)
    , m_nStartTime(0)
    , m_nFinishTime(0)
    , m_biModeStartTime(0)
    , m_biModeEndTime(0)
    , m_pBattleZone(nullptr)
    , m_nMonsterClearCount(0)
    , m_bSuccess(false)
{
    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode constructed");
}

// 对齐 IDA: CGameWorldMode 析构函数
CGameWorldMode::~CGameWorldMode() {
    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode destructed");
}

// 对齐 IDA: Init 初始化
void CGameWorldMode::Init(CBattleZone* pBattleZone, ST_WORLD_MODE_INFO* pstInfo) {
    if (!pBattleZone || !pstInfo) {
        return;
    }

    m_pBattleZone = pBattleZone;
    m_nModeID = pstInfo->nModeID;
    m_nModeDateID = pstInfo->nModeDateID;
    m_nStartTime = pstInfo->nStartTime;
    m_nFinishTime = pstInfo->nFinishTime;
    m_biModeStartTime = pstInfo->biModeStartTime;
    m_biModeEndTime = pstInfo->biModeEndTime;
    m_nState = pstInfo->nState;
    m_nMonsterClearCount = pstInfo->nMonsterClearCount;
    m_bSuccess = pstInfo->bSuccess;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::Init");
}

// 对齐 IDA: StartMode 启动模式
void CGameWorldMode::StartMode() {
    m_nState = 1;  // 进行中

    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::StartMode");
}

// 对齐 IDA: FinishMode 完成模式
void CGameWorldMode::FinishMode(PS_WORLD_MODE_FINISH* pstFinish) {
    if (!pstFinish) {
        return;
    }

    m_nState = 2;  // 已完成
    m_nMonsterClearCount = pstFinish->nMonsterClearCount;
    m_bSuccess = pstFinish->bSuccess;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::FinishMode");
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
    if (m_nState != 1) {
        return;
    }

    // 检查是否超时
    if (m_biModeEndTime > 0 && nCurrentTime >= m_biModeEndTime) {
        m_nState = 2;  // 已完成
        m_bSuccess = false;  // 超时失败
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::Update");
}

// 对齐 IDA: Sync 同步到客户端
void CGameWorldMode::Sync() {
    // TODO: 实现同步到客户端的逻辑
    // 需要发送当前模式状态、怪物数量等信息给客户端

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
    m_nModeID = 0;
    m_nModeDateID = 0;
    m_nState = 0;
    m_nStartTime = 0;
    m_nFinishTime = 0;
    m_biModeStartTime = 0;
    m_biModeEndTime = 0;
    m_nMonsterClearCount = 0;
    m_bSuccess = false;
    m_nModeType = 0;
    m_listMonster.clear();

    GreenDamTan_log(__FILE__, __FUNCTION__, "CGameWorldMode::Reset");
}
