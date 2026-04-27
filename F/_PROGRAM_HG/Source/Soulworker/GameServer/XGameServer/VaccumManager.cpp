#include "Soulworker/GameServer/XGameServer/VaccumManager.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"

CVaccumManager::CVaccumManager()
    : m_pArea(nullptr)
    , m_bAutoSpawn(false)
{
}

CVaccumManager::~CVaccumManager() {
}

void CVaccumManager::Init(CBattleZone* pArea, bool bAutoSpawn) {
    m_pArea = pArea;
    m_bAutoSpawn = bAutoSpawn;
}

void CVaccumManager::Clear() {
    m_mapVaccumGroup.clear();
    m_mapVaccumTableID.clear();
    m_vecBoxID.clear();
    m_mapVaccumCheat.clear();
    m_mapVaccumNoneAuto.clear();
}

bool CVaccumManager::AddVaccumGroup(int nID, std::tr1::shared_ptr<CVaccumGroup> pGroup) {
    // TODO: 实现
    return false;
}

void CVaccumManager::RemoveVaccumGroup(int nID) {
    m_mapVaccumGroup.erase(nID);
}

CVaccumGroup* CVaccumManager::FindVaccumGroup(int nID) {
    // TODO: 实现
    return nullptr;
}

void CVaccumManager::SpawnAll() {
    // TODO: 实现
}

void CVaccumManager::OnUpdate(float fDelta) {
    // TODO: 实现
}
