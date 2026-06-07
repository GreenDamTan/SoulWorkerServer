// DefenceMode.h
// DefenceMode - Defence mode class for game modes
// IDA: ?Init@DefenceMode@@QEAAXXZ (0x1402a58d0)
// IDA: ?CheckEndMode@DefenceMode@@UEAA_NXZ (0x1402a5f00)

#pragma once

#include "GameModeBase.h"
#include <cstdint>

// Forward declarations
struct TB_MODE_DEFENCE;
class CSector;
class CMonster;

// DefenceMode - Defence game mode
class DefenceMode : public GameModeBase {
public:
    DefenceMode() : GameModeBase() {
        Init();
    }
    ~DefenceMode() override = default;

    // IDA: ?Init@DefenceMode@@QEAAXXZ (0x1402a58d0)
    // Initializes the defence mode
    void Init() {
        GameModeBase::Init();
        m_pModeTableRef = nullptr;
        m_pSpawnTableMap = nullptr;
        m_pDefenceActor = nullptr;
        m_fPulseAggroTime = 0.0f;
        m_iSectorID = -1;
        m_fLastSpawnTime = 0.0f;
        m_fFullModeTime = 0.0f;
        m_iSectorUniqueID = -1;
    }

    // IDA: ?CheckEndMode@DefenceMode@@UEAA_NXZ (0x1402a5f00)
    // Checks if the defence mode should end
    virtual bool CheckEndMode() {
        // Check if defence actor is dead
        if (m_pDefenceActor && m_pDefenceActor->GetHP() <= 0) {
            EndGame(2);  // Failed - defence actor died
            return true;
        }

        // TODO: Full implementation requires:
        // - Scanning actors in sector
        // - Checking user/monster alive status
        // - Calling EndGame with appropriate result code

        return false;
    }

    // EndGame - Ends the defence mode with result
    virtual void EndGame(std::uint8_t byResult) {
        m_byModeResult = byResult;
        m_bPlayMode = 0;
    }

    // AllMonsterSpawned - Check if all monsters have spawned
    virtual bool AllMonsterSpawned() const { return true; }  // Simplified

    // Accessors
    TB_MODE_DEFENCE* GetModeTableRef() const { return m_pModeTableRef; }
    void SetModeTableRef(TB_MODE_DEFENCE* pTable) { m_pModeTableRef = pTable; }

    CSector* GetDefenceActor() const { return m_pDefenceActor; }
    void SetDefenceActor(CSector* pActor) { m_pDefenceActor = pActor; }

    int GetSectorID() const { return m_iSectorID; }
    void SetSectorID(int nID) { m_iSectorID = nID; }

    int GetSectorUniqueID() const { return m_iSectorUniqueID; }
    void SetSectorUniqueID(int nID) { m_iSectorUniqueID = nID; }

    float GetPulseAggroTime() const { return m_fPulseAggroTime; }
    void SetPulseAggroTime(float fTime) { m_fPulseAggroTime = fTime; }

    float GetLastSpawnTime() const { return m_fLastSpawnTime; }
    void SetLastSpawnTime(float fTime) { m_fLastSpawnTime = fTime; }

    float GetFullModeTime() const { return m_fFullModeTime; }
    void SetFullModeTime(float fTime) { m_fFullModeTime = fTime; }

protected:
    TB_MODE_DEFENCE* m_pModeTableRef;    // Mode table reference
    void* m_pSpawnTableMap;              // Spawn table map (TODO: define type)
    CSector* m_pDefenceActor;            // Defence actor (sector)
    float m_fPulseAggroTime;             // Pulse aggro time
    int m_iSectorID;                     // Sector ID
    float m_fLastSpawnTime;              // Last spawn time
    float m_fFullModeTime;               // Full mode time
    int m_iSectorUniqueID;               // Sector unique ID
};
