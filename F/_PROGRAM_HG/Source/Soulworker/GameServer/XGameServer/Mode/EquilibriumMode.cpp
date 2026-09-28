#include "Soulworker/GameServer/XGameServer/EquilibriumMode.h"
#include "Soulworker/GameServer/XGameServer/Maze.h"
#include "Soulworker/GameServer/XGameServer/Sector.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XCommon/VEventObjectDefine.h"

EquilibriumMode::EquilibriumMode() : GameModeBase() {
}

EquilibriumMode::~EquilibriumMode() {
}

void EquilibriumMode::Init(STMageCheckEventSpawnBox* pEventSpawnInfo) {
    GameModeBase::Init();
    m_pEventSpawnInfo = pEventSpawnInfo;
    m_fLastSpawnTime = 0.0f;
    m_fFullModeTime = 0.0f;
    m_nModeState = 0;
    m_eModeType = eGM_MODE_EQ;
    m_nSectorID = 0;
}

void EquilibriumMode::StartMode(XMaze* pMaze) {
    if (!m_bPlayMode) {
        m_bPlayMode = true;
        m_pMaze = pMaze;
        VMonsterSpawnInfo* pMonsterSpawnInfo = m_pMaze->GetProcessSpawnBoxInfo(
            static_cast<std::uint16_t>(m_pEventSpawnInfo->pEventBox->m_iSpawn_Box_ID[0]));
        if (pMonsterSpawnInfo) {
            m_nSectorID = pMonsterSpawnInfo->m_iSectorID;
            m_nMonster = pMonsterSpawnInfo->m_stMonsterInfo[0].m_iID;
        }
        CSector* pSector = m_pMaze->GetSector(m_nSectorID);
        if (pSector) {
            pSector->SetMode(this);
            m_pMaze->SetEventSector(pSector);
        }
        SendNoticePacket(pMaze, 21, 0, m_pEventSpawnInfo->pEventBox->m_fEvent_Time / 1000.0f, false);
    }
}

void EquilibriumMode::Tick(float fDeltaTime) {
    if (m_bPlayMode) {
        int nModeState = m_nModeState;
        if (nModeState) {
            if (nModeState == 1) {
                m_fFullModeTime = m_fFullModeTime + fDeltaTime;
                if (m_fFullModeTime >= m_pEventSpawnInfo->pEventBox->m_fEvent_Time / 1000.0f) {
                    m_nModeState = 4;
                    m_bPlayMode = false;
                }
            }
        } else {
            float fDelayTime = m_pEventSpawnInfo->fDelayTime - fDeltaTime;
            m_pEventSpawnInfo->fDelayTime = fDelayTime;
            if (fDelayTime <= 0.0f) {
                m_nModeState = 1;
                StartSpawnMonster();
                m_pMaze->DieEventSectorMonster(0, true);
            }
        }
    }
}

void EquilibriumMode::StartSpawnMonster() {
    for (int i = 0; i < 5; ++i) {
        int nSpawnIndex = m_pEventSpawnInfo->pEventBox->m_iSpawn_Box_ID[i];
        VMonsterSpawnInfo* pMonsterSpawnInfo = m_pMaze->GetProcessSpawnBoxInfo(
            static_cast<std::uint16_t>(nSpawnIndex));
        if (pMonsterSpawnInfo) {
            m_pMaze->ExcuteSpawnBoxCheck(nSpawnIndex, eSendInfoTypeSend, false);
            float fLastSpawnTime;
            if (m_fLastSpawnTime <= pMonsterSpawnInfo->m_fWaitCreationDelayTime
                    + pMonsterSpawnInfo->m_fWaitCreationSequenceTime
                        * static_cast<float>(pMonsterSpawnInfo->m_iWaitCreationMaxWave - 1) + 1.0f) {
                fLastSpawnTime = pMonsterSpawnInfo->m_fWaitCreationDelayTime
                    + pMonsterSpawnInfo->m_fWaitCreationSequenceTime
                        * static_cast<float>(pMonsterSpawnInfo->m_iWaitCreationMaxWave - 1) + 1.0f;
            } else {
                fLastSpawnTime = m_fLastSpawnTime;
            }
            m_fLastSpawnTime = fLastSpawnTime;
        }
    }
}

bool EquilibriumMode::AllMonsterSpawned() {
    return m_fFullModeTime >= m_fLastSpawnTime;
}

bool EquilibriumMode::CheckEndMode() {
    if (!AllMonsterSpawned()) {
        return false;
    }
    if (!m_pMaze) {
        return true;
    }
    CSector* pSector = m_pMaze->GetSector(m_nSectorID);
    if (pSector) {
        if (!pSector->GetMonsterCount()) {
            EndGame(0);
            return true;
        }
        if (m_nModeState == 4) {
            EndGame(1);
            return true;
        }
        if (m_nModeState == 3) {
            EndGame(0);
            return true;
        }
    }
    return false;
}

void EquilibriumMode::EndGame(std::uint8_t byResult) {
    if (byResult == 1) {
        CSector* pSector = m_pMaze->GetSector(m_nSectorID);
        if (pSector) {
            pSector->DieMonsters(3, true);
        }
    }
    m_pMaze->SetEventSector(nullptr);
    SendNoticePacket(m_pMaze, 22, byResult, 0.0f, false);
}

void EquilibriumMode::Intrusion(CUser* pUser) {
    float fRemain = m_pEventSpawnInfo->pEventBox->m_fEvent_Time / 1000.0f - m_fFullModeTime;
    LogHelper::LogDebug("game.contents", "<EQ> Intrusion ( %2.f)", fRemain);
    SendNoticePacket(pUser, 21, 1, fRemain);
}
