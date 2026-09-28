// EquilibriumMode.h
// EquilibriumMode - Equilibrium game mode class
// IDA: ?Init@EquilibriumMode@@QEAAXPEAUSTMageCheckEventSpawnBox@@@Z (0x1402a6ae0)

#pragma once

#include "GameModeBase.h"
#include <cstdint>

// Forward declarations
struct STMageCheckEventSpawnBox;

// EquilibriumMode - Equilibrium game mode
class EquilibriumMode : public GameModeBase {
public:
    EquilibriumMode();
    ~EquilibriumMode() override;

    void Init(STMageCheckEventSpawnBox* pEventSpawnInfo);
    // TODO: 需人工审查：StartMode (0x1402a6b50) 使用了 PDB 的 CSector::SetMode 类型；
    // 原始 GameModeMgr 注册链与事件广播运行路径仍未验证。
    void StartMode(XMaze* pMaze) override;
    void Tick(float fDelta) override;
    bool CheckEndMode() override;
    void Intrusion(CUser* pUser) override;

    void StartSpawnMonster();
    bool AllMonsterSpawned();
    void EndGame(std::uint8_t byResult);

    STMageCheckEventSpawnBox* GetEventSpawnInfo() const { return m_pEventSpawnInfo; }
    void SetEventSpawnInfo(STMageCheckEventSpawnBox* pInfo) { m_pEventSpawnInfo = pInfo; }
    int GetModeState() const { return m_nModeState; }
    void SetModeState(int nState) override { m_nModeState = nState; }
    eGAMEMODE_TYPE GetModeType() const { return m_eModeType; }
    void SetModeType(eGAMEMODE_TYPE eType) { m_eModeType = eType; }
    int GetSectorID() const { return m_nSectorID; }
    void SetSectorID(int nID) { m_nSectorID = nID; }
    float GetLastSpawnTime() const { return m_fLastSpawnTime; }
    void SetLastSpawnTime(float fTime) { m_fLastSpawnTime = fTime; }
    float GetFullModeTime() const { return m_fFullModeTime; }
    void SetFullModeTime(float fTime) { m_fFullModeTime = fTime; }

protected:
    STMageCheckEventSpawnBox* m_pEventSpawnInfo;
    float m_fLastSpawnTime;
    float m_fFullModeTime;
    int m_nSectorID;
    int m_nMonster;
};

static_assert(sizeof(EquilibriumMode) == 112, "EquilibriumMode size must match GameServer PDB");
