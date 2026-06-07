// GameModeMgr.cpp
// GameModeMgr implementation
// Reconstructed from IDA decompilation: GameServer.exe
//
// Functions:
// - GameModeMgr::Init (0x1402a79e0)

#include "GameModeMgr.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

// Forward declarations - placeholder types
class DefenceMode {
public:
    void Init(int nSize) {}
};
class SurvivalMode {
public:
    void Init(int nSize) {}
};
class OperationMode {
public:
    void Init(int nSize) {}
};

// ============================================================================
// GameModeMgr implementation
// ============================================================================

GameModeMgr::GameModeMgr()
    : m_fEndGameCheckTime(0.0f)
{
}

GameModeMgr::~GameModeMgr()
{
}

// Init - Initialize all game mode managers
// IDA @ 0x1402A79E0
void GameModeMgr::Init(int nSize)
{
    // IDA code:
    // void __fastcall GameModeMgr::Init(GameModeMgr *this, int nSize)
    // {
    //   this->m_fEndGameCheckTime = 0.0;
    //   this->m_xDefenceMode.Init(&this->m_xDefenceMode, nSize);
    //   this->m_xSurvivalMode.Init(&this->m_xSurvivalMode, nSize);
    //   this->m_xOperationMode.Init(&this->m_xOperationMode, nSize);
    // }

    m_fEndGameCheckTime = 0.0f;
    m_xDefenceMode.Init(nSize);
    m_xSurvivalMode.Init(nSize);
    m_xOperationMode.Init(nSize);
}
