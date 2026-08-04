// DohHavokResourceManager.h
// Havok Resource Manager - manages navmesh resources
// Precisely restored from IDA decompilation

#pragma once

#include <cstdint>
#include <map>
#include <string>

// Forward declarations
class HavokNavMeshResource;

// ============================================================================
// DohHavokResourceManager - Havok navigation mesh resource manager
// IDA: ??0DohHavokResourceManager@@QEAA@XZ (0x1402f8fe0)
// ============================================================================
class DohHavokResourceManager {
public:
    DohHavokResourceManager();
    virtual ~DohHavokResourceManager();

    // IDA: loadNavMesh - load navigation mesh from file
    // Returns HavokNavMeshResource pointer, nullptr on failure
    HavokNavMeshResource* loadNavMesh(const char* szFilePath);

private:
    // IDA: m_resources - map of file path to navmesh resource
    std::map<std::string, HavokNavMeshResource*> m_resources;
};
