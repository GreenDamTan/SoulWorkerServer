#include "Soulworker/GameServer/XGameServer/RespawnManager.h"

CRespawnManager::CRespawnManager()
    : m_bPause(false)
{
}

CRespawnManager::~CRespawnManager() {
}

void CRespawnManager::Clear() {
    m_mapRespawnWaitObject.clear();
    m_mapRespawnObject.clear();
}

void CRespawnManager::Pause(bool bPause) {
    m_bPause = bPause;
}

bool CRespawnManager::AddRespawnObject(int nID, ST_RESPAWN_OBJECT& stObject) {
    // TODO: 实现
    return false;
}

void CRespawnManager::RemoveRespawnObject(int nID) {
    m_mapRespawnObject.erase(nID);
}

ST_RESPAWN_OBJECT* CRespawnManager::FindRespawnObject(int nID) {
    // TODO: 实现
    return nullptr;
}

void CRespawnManager::OnUpdate(float fDelta) {
    if (m_bPause) return;
    // TODO: 实现
}
