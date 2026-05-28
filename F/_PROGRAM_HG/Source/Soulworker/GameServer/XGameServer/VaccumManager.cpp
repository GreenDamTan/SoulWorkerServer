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

bool CVaccumManager::AddVaccumGroup(int nID, std::shared_ptr<CVaccumGroup> pGroup) {
    if (!pGroup) {
        return false;
    }

    auto result = m_mapVaccumGroup.insert(std::make_pair(nID, pGroup));
    return result.second;
}

void CVaccumManager::RemoveVaccumGroup(int nID) {
    m_mapVaccumGroup.erase(nID);
}

CVaccumGroup* CVaccumManager::FindVaccumGroup(int nID) {
    auto it = m_mapVaccumGroup.find(nID);
    if (it != m_mapVaccumGroup.end()) {
        return static_cast<CVaccumGroup*>(it->second.get());
    }
    return nullptr;
}

void CVaccumManager::SpawnAll() {
    if (!m_pArea) {
        return;
    }

    // IDA confirmed: iterate all vaccum groups and trigger spawning
    for (auto& pair : m_mapVaccumGroup) {
        CVaccumGroup* pGroup = static_cast<CVaccumGroup*>(pair.second.get());
        if (pGroup) {
            // Per IDA: each group has spawn logic
            // For now, placeholder until CVaccumGroup is fully defined
        }
    }

    // Also spawn non-auto groups
    for (auto& pair : m_mapVaccumNoneAuto) {
        CVaccumGroup* pGroup = static_cast<CVaccumGroup*>(pair.second.get());
        if (pGroup) {
            // Per IDA: spawn logic for non-auto groups
        }
    }
}

void CVaccumManager::OnUpdate(float fDelta) {
    if (!m_pArea) {
        return;
    }

    // IDA confirmed: update all vaccum groups
    for (auto& pair : m_mapVaccumGroup) {
        CVaccumGroup* pGroup = static_cast<CVaccumGroup*>(pair.second.get());
        if (pGroup) {
            // Per IDA: each group has update logic
            // Placeholder until CVaccumGroup::OnUpdate is defined
        }
    }

    // Update non-auto groups as well
    for (auto& pair : m_mapVaccumNoneAuto) {
        CVaccumGroup* pGroup = static_cast<CVaccumGroup*>(pair.second.get());
        if (pGroup) {
            // Per IDA: update logic for non-auto groups
        }
    }
}

// ============================================================================
// Update IDA 0x140191730
// 更新真空区域管理器
// ============================================================================
void CVaccumManager::Update() {
    // IDA 反编译确认: 调用各真空组更新逻辑
    OnUpdate(0.0f);
}

// ============================================================================
// Add - Add vaccum entry (wrapper for AddVaccumGroup)
// ============================================================================
bool CVaccumManager::Add(int nID) {
    std::shared_ptr<void> pEmpty;
    auto result = m_mapVaccumGroup.insert(std::make_pair(nID, pEmpty));
    return result.second;
}

// ============================================================================
// Remove - Remove vaccum entry
// ============================================================================
bool CVaccumManager::Remove(int nID) {
    auto it = m_mapVaccumGroup.find(nID);
    if (it != m_mapVaccumGroup.end()) {
        m_mapVaccumGroup.erase(it);
        return true;
    }
    return false;
}

// ============================================================================
// Process - Process vaccum logic
// ============================================================================
void CVaccumManager::Process() {
    Update();
}

// ============================================================================
// GetCount - Get entry count
// ============================================================================
int CVaccumManager::GetCount() const {
    return static_cast<int>(m_mapVaccumGroup.size());
}

// ============================================================================
// IsActive - Check if active
// ============================================================================
bool CVaccumManager::IsActive() const {
    return !m_mapVaccumGroup.empty();
}

// ============================================================================
// Start - Start vaccum
// ============================================================================
bool CVaccumManager::Start(int nID) {
    return Add(nID);
}

// ============================================================================
// Stop - Stop vaccum
// ============================================================================
bool CVaccumManager::Stop(int nID) {
    return Remove(nID);
}

// ============================================================================
// GetPosition - Get vaccum position (placeholder)
// ============================================================================
void CVaccumManager::GetPosition(int nID, float* pX, float* pY, float* pZ) {
    if (pX) *pX = 0.0f;
    if (pY) *pY = 0.0f;
    if (pZ) *pZ = 0.0f;
}

// ============================================================================
// SetPosition - Set vaccum position
// ============================================================================
void CVaccumManager::SetPosition(int nID, float fX, float fY, float fZ) {
    auto it = m_mapVaccumGroup.find(nID);
    if (it != m_mapVaccumGroup.end()) {
        CVaccumGroup* pGroup = static_cast<CVaccumGroup*>(it->second.get());
        if (pGroup) {
            // Per IDA: update group position
            // Placeholder until CVaccumGroup::SetPosition is defined
        }
    }
}
