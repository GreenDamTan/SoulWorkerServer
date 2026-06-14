// CellPosMgr.cpp
// IDA: Cell position manager implementation
// Exact restoration from IDA decompilation

#include "CellPosMgr.h"

// ============================================================================
// Constructor
// IDA: 0x140194970
// ============================================================================
CCellPosMgr::CCellPosMgr()
    : m_vCenterPos(0.0f, 0.0f, 0.0f)
{
    // Maps are default constructed
}

// ============================================================================
// Destructor
// IDA: 0x140354490
// ============================================================================
CCellPosMgr::~CCellPosMgr()
{
    RemoveAllInfo();
}

// ============================================================================
// RemoveAllInfo
// IDA: 0x140194ED0
// Clear all cell and monster mappings
// ============================================================================
void CCellPosMgr::RemoveAllInfo()
{
    m_mapCellToMonster.clear();
    m_mapMonsterToCell.clear();
}

// ============================================================================
// CellIDFromPos
// IDA: 0x140194F00
// Convert world position to cell ID
// Returns: (cellX << 16) | cellY where cellX/Y = (pos - center) / 80.0
// ============================================================================
std::uint32_t CCellPosMgr::CellIDFromPos(hkvVec3& vPos)
{
    hkvVec3 vDiff = vPos - m_vCenterPos;

    // Cell coordinates: x and y divided by 80
    std::int16_t cellX = static_cast<std::int16_t>(static_cast<int>(vDiff.x / CELL_SIZE));
    std::int16_t cellY = static_cast<std::int16_t>(static_cast<int>(vDiff.y / CELL_SIZE));

    // Pack into 32-bit ID: high word = X, low word = Y
    return (static_cast<std::uint32_t>(cellX) << 16) | static_cast<std::uint32_t>(cellY);
}

std::uint32_t CCellPosMgr::CellIDFromPos(const hkvVec3& vPos) const
{
    hkvVec3 vDiff = vPos - m_vCenterPos;

    std::int16_t cellX = static_cast<std::int16_t>(static_cast<int>(vDiff.x / CELL_SIZE));
    std::int16_t cellY = static_cast<std::int16_t>(static_cast<int>(vDiff.y / CELL_SIZE));

    return (static_cast<std::uint32_t>(cellX) << 16) | static_cast<std::uint32_t>(cellY);
}

// ============================================================================
// CellIDToPos
// IDA: 0x140194F70
// Convert cell ID back to world position (center of cell)
// ============================================================================
hkvVec3 CCellPosMgr::CellIDToPos(std::uint32_t dwCellID) const
{
    std::int16_t cellX = static_cast<std::int16_t>(dwCellID >> 16);
    std::int16_t cellY = static_cast<std::int16_t>(dwCellID & 0xFFFF);

    float worldX = static_cast<float>(cellX) * CELL_SIZE + m_vCenterPos.x;
    float worldY = static_cast<float>(cellY) * CELL_SIZE + m_vCenterPos.y;

    return hkvVec3(worldX, worldY, m_vCenterPos.z);
}

// ============================================================================
// ExistMonsterAtPos
// IDA: 0x140194D20
// Check if a monster exists at the given cell ID
// ============================================================================
bool CCellPosMgr::ExistMonsterAtPos(std::uint32_t dwCellID) const
{
    return m_mapCellToMonster.find(dwCellID) != m_mapCellToMonster.end();
}

// ============================================================================
// GetEmptyCellID
// IDA: 0x1401949D0
// Find an empty cell near the given position
// If the cell at vPos is occupied, search in a spiral pattern
// Sets bDuplicated to 1 if found in a different cell, 0 if original cell is empty
// ============================================================================
std::uint32_t CCellPosMgr::GetEmptyCellID(hkvVec3& vPos, int* bDuplicated)
{
    std::uint32_t dwCellID = CellIDFromPos(vPos);

    // Check if cell is empty
    if (!ExistMonsterAtPos(dwCellID))
    {
        *bDuplicated = 0;
        return dwCellID;
    }

    // Cell is occupied, search in spiral pattern
    *bDuplicated = 1;

    // Extract cell coordinates
    std::int16_t nX = static_cast<std::int16_t>(dwCellID >> 16);
    std::int16_t nY = static_cast<std::int16_t>(dwCellID & 0xFFFF);

    // Search offsets in spiral order (24 directions)
    // Each pair is (deltaX, deltaY)
    static const std::int16_t s_offsets[24][2] = {
        { 0,  1},   // North
        { 1,  1},   // NE
        { 1,  0},   // East
        { 1, -1},   // SE
        { 0, -1},   // South
        {-1, -1},   // SW
        {-1,  0},   // West
        {-1,  1},   // NW
        { 0,  2},   // N2
        { 1,  2},   // NE2
        { 2,  2},   // ENE2
        { 2,  1},   // E2
        { 2,  0},   // E2
        { 2, -1},   // SE2
        { 2, -2},   // SSE2
        { 1, -2},   // S2
        { 0, -2},   // S2
        {-1, -2},   // SW2
        {-2, -2},   // WSW2
        {-2, -1},   // W2
        {-2,  0},   // W2
        {-2,  1},   // NW2
        {-2,  2},   // WNW2
        {-1,  2},   // N2
    };

    for (int i = 0; i < 24; ++i)
    {
        std::int16_t newX = nX + s_offsets[i][0];
        std::int16_t newY = nY + s_offsets[i][1];
        std::uint32_t newCellID = (static_cast<std::uint32_t>(newX) << 16) | static_cast<std::uint32_t>(newY);

        if (!ExistMonsterAtPos(newCellID))
        {
            return newCellID;
        }
    }

    // No empty cell found
    return 0xFFFFFFFF;
}

// ============================================================================
// AddMonsterInfo
// IDA: 0x140194D80
// Add mapping: cell -> monster ID and monster ID -> cell
// ============================================================================
void CCellPosMgr::AddMonsterInfo(std::uint32_t dwCellID, std::uint32_t dwID)
{
    // First remove any existing mapping for this monster ID
    RemoveMonsterInfo(dwID);

    // Add new mappings
    m_mapCellToMonster[dwCellID] = dwID;
    m_mapMonsterToCell[dwID] = dwCellID;
}

// ============================================================================
// RemoveMonsterInfo
// IDA: 0x140194DE0
// Remove monster from both maps
// ============================================================================
void CCellPosMgr::RemoveMonsterInfo(std::uint32_t dwID)
{
    // Find the cell ID for this monster
    auto it = m_mapMonsterToCell.find(dwID);
    if (it != m_mapMonsterToCell.end())
    {
        std::uint32_t dwCellID = it->second;

        // Remove from cell -> monster map
        auto itCell = m_mapCellToMonster.find(dwCellID);
        if (itCell != m_mapCellToMonster.end())
        {
            m_mapCellToMonster.erase(itCell);
        }

        // Remove from monster -> cell map
        m_mapMonsterToCell.erase(it);
    }
}
