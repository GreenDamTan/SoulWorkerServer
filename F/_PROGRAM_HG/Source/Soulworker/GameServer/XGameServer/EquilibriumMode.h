// EquilibriumMode.h
// EquilibriumMode - Equilibrium game mode class
// IDA: ?Init@EquilibriumMode@@QEAAXPEAUSTMageCheckEventSpawnBox@@@Z (0x1402a6ae0)

#pragma once

#include "GameModeBase.h"
#include <cstdint>

// Forward declarations
struct STMageCheckEventSpawnBox;

// E_GAME_MODE_TYPE - Game mode type enumeration
enum E_GAME_MODE_TYPE {
    eGM_MODE_NONE = 0,
    eGM_MODE_DEFENCE = 1,
    eGM_MODE_SURVIVAL = 2,
    eGM_MODE_OPERATION = 3,
    eGM_MODE_UNITY = 4,
    eGM_MODE_WARLORD = 5,
    eGM_MODE_EQ = 6
};

// EquilibriumMode - Equilibrium game mode
class EquilibriumMode : public GameModeBase {
public:
    EquilibriumMode() : GameModeBase() {
        Init(nullptr);
    }
    ~EquilibriumMode() override = default;

    // IDA: ?Init@EquilibriumMode@@QEAAXPEAUSTMageCheckEventSpawnBox@@@Z (0x1402a6ae0)
    // Initializes the equilibrium mode with event spawn info
    void Init(STMageCheckEventSpawnBox* pEventSpawnInfo) {
        GameModeBase::Init();
        m_pEventSpawnInfo = pEventSpawnInfo;
        m_fLastSpawnTime = 0.0f;
        m_fFullModeTime = 0.0f;
        m_nModeState = 0;
        m_eModeType = eGM_MODE_EQ;
        m_nSectorID = 0;
    }

    // Accessors
    STMageCheckEventSpawnBox* GetEventSpawnInfo() const { return m_pEventSpawnInfo; }
    void SetEventSpawnInfo(STMageCheckEventSpawnBox* pInfo) { m_pEventSpawnInfo = pInfo; }

    int GetModeState() const { return m_nModeState; }
    void SetModeState(int nState) { m_nModeState = nState; }

    E_GAME_MODE_TYPE GetModeType() const { return m_eModeType; }
    void SetModeType(E_GAME_MODE_TYPE eType) { m_eModeType = eType; }

    int GetSectorID() const { return m_nSectorID; }
    void SetSectorID(int nID) { m_nSectorID = nID; }

    float GetLastSpawnTime() const { return m_fLastSpawnTime; }
    void SetLastSpawnTime(float fTime) { m_fLastSpawnTime = fTime; }

    float GetFullModeTime() const { return m_fFullModeTime; }
    void SetFullModeTime(float fTime) { m_fFullModeTime = fTime; }

private:
    STMageCheckEventSpawnBox* m_pEventSpawnInfo;
    float m_fLastSpawnTime;
    float m_fFullModeTime;
    int m_nModeState;
    E_GAME_MODE_TYPE m_eModeType;
    int m_nSectorID;
};
