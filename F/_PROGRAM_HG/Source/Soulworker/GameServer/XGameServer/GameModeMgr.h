// GameModeMgr.h
// GameModeMgr - Game mode manager
// IDA decompilation from GameServer.exe
//
// Functions:
// - GameModeMgr::Init (0x1402a79e0)

#pragma once

#include <cstdint>

// Forward declarations
class DefenceMode;
class SurvivalMode;
class OperationMode;

// ============================================================================
// GameModeMgr - Game mode manager
// Manages different game modes (Defence, Survival, Operation)
// ============================================================================
class GameModeMgr {
public:
    // Constructor
    GameModeMgr();

    // Destructor
    ~GameModeMgr();

    // Init - Initialize all game mode managers
    // IDA: ?Init@GameModeMgr@@QEAAXH@Z @ 0x1402A79E0
    void Init(int nSize);

    // Accessors
    DefenceMode* GetDefenceMode() { return &m_xDefenceMode; }
    SurvivalMode* GetSurvivalMode() { return &m_xSurvivalMode; }
    OperationMode* GetOperationMode() { return &m_xOperationMode; }

protected:
    // === Member Variables ===
    DefenceMode m_xDefenceMode;       // Defence mode manager
    SurvivalMode m_xSurvivalMode;     // Survival mode manager
    OperationMode m_xOperationMode;   // Operation mode manager
    float m_fEndGameCheckTime;        // End game check timer
};
