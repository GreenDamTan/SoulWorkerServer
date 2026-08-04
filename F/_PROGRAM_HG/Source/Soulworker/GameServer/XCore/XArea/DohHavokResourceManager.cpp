// DohHavokResourceManager.cpp
// Havok Resource Manager - manages navmesh resources
// Precisely restored from IDA decompilation

#include "DohHavokResourceManager.h"
#include "../HavokTypes.h"
#include <cstring>

// ============================================================================
// DohHavokResourceManager::DohHavokResourceManager
// IDA: ??0DohHavokResourceManager@@QEAA@XZ (0x1402f8fe0)
// ============================================================================
DohHavokResourceManager::DohHavokResourceManager() {
    // IDA: Initialize resource map
    // std::map<std::string, HavokNavMeshResource*>::map()
}

// ============================================================================
// DohHavokResourceManager::~DohHavokResourceManager
// IDA: ??1DohHavokResourceManager@@UEAA@XZ (0x1402f9080)
// ============================================================================
DohHavokResourceManager::~DohHavokResourceManager() {
    // IDA: Clear resource map
    for (auto& pair : m_resources) {
        if (pair.second) {
            hkReferencedObject::removeReference(
                reinterpret_cast<hkReferencedObject*>(pair.second));
        }
    }
    m_resources.clear();
}

// ============================================================================
// DohHavokResourceManager::loadNavMesh
// IDA: Load navigation mesh from file
// ============================================================================
HavokNavMeshResource* DohHavokResourceManager::loadNavMesh(const char* szFilePath) {
    if (!szFilePath || szFilePath[0] == '\0') {
        return nullptr;
    }

    // Check if already loaded
    auto it = m_resources.find(szFilePath);
    if (it != m_resources.end()) {
        return it->second;
    }

    // IDA: Load navmesh from file
    // Stub - actual Havok navmesh loading requires Havok SDK
    // HavokNavMeshResource* pResource = hkAssetManagement::loadNavMesh(szFilePath);

    // For now, return nullptr as we don't have actual Havok SDK
    // When Havok SDK is available, this should load the actual navmesh file
    return nullptr;
}
