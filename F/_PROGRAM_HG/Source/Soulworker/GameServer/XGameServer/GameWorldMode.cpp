// GameWorldMode.cpp
// CGameWorldMode 游戏世界模式类实现
// 对齐 IDA GameServer.exe

#include "Soulworker/GameServer/XGameServer/GameWorldMode.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

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
