#pragma once

#include <map>
#include <cstdint>
#include "Soulworker/GameServer/XCore/VisionEngineTypes/hkvVec3.h"

// ============================================================================
// CCellPosMgr - Cell position manager for tracking monster positions
// IDA: 72 bytes (estimated from map sizes)
// Used to track and manage monster positions in grid cells for collision avoidance
// ============================================================================
class CCellPosMgr
{
public:
    CCellPosMgr();
    ~CCellPosMgr();

    // Remove all tracked info
    void RemoveAllInfo();

    // Get cell ID from position
    std::uint32_t CellIDFromPos(hkvVec3& vPos);

    // Get cell ID from position (const version)
    std::uint32_t CellIDFromPos(const hkvVec3& vPos) const;

    // Convert cell ID to position
    hkvVec3 CellIDToPos(std::uint32_t dwCellID) const;

    // Check if monster exists at cell position
    bool ExistMonsterAtPos(std::uint32_t dwCellID) const;

    // Get empty cell ID near position
    std::uint32_t GetEmptyCellID(hkvVec3& vPos, int* bDuplicated);

    // Add monster info (cell -> monster ID mapping)
    void AddMonsterInfo(std::uint32_t dwCellID, std::uint32_t dwID);

    // Remove monster info by monster ID
    void RemoveMonsterInfo(std::uint32_t dwID);

    // Set center position for grid calculations
    void SetCenterPos(const hkvVec3& vPos) { m_vCenterPos = vPos; }
    const hkvVec3& GetCenterPos() const { return m_vCenterPos; }

private:
    // Cell size for grid calculations
    static constexpr float CELL_SIZE = 80.0f;

    // Cell to monster ID mapping
    std::map<std::uint32_t, std::uint32_t> m_mapCellToMonster;

    // Monster ID to cell mapping (reverse lookup)
    std::map<std::uint32_t, std::uint32_t> m_mapMonsterToCell;

    // Center position for grid coordinate calculations
    hkvVec3 m_vCenterPos;
};
