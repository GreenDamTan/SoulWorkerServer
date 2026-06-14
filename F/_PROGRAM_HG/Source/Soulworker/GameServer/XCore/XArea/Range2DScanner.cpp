// Range2DScanner.cpp
// Implementation of 2D Grid-based spatial partitioning scanner
// Reconstructed from IDA decompilation

#include "Soulworker/GameServer/XCore/XArea/Range2DScanner.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <cmath>

// ============================================================================
// UniformGrid Implementation
// ============================================================================

template<typename T>
UniformGrid<T>::UniformGrid(int width, int height)
    : m_pVecGridArray(nullptr)
    , m_nWidthGrid(width)
    , m_nHeightGrid(height)
    , m_unSize(0)
{
    // IDA: ??0?$UniformGrid@PEAVCMover@@@@QEAA@HH@Z
    // Allocate grid array
    int size = width * height;
    m_pVecGridArray = new std::vector<T>[size];
}

template<typename T>
UniformGrid<T>::~UniformGrid() {
    if (m_pVecGridArray) {
        delete[] m_pVecGridArray;
        m_pVecGridArray = nullptr;
    }
}

template<typename T>
void UniformGrid<T>::EnumerateAll(std::vector<T>& vecOut) {
    // IDA: ?EnumerateAll@?$UniformGrid@PEAVCMover@@@@QEAAXAEAV?$vector@PEAVCMover@@...
    // Verified from IDA decompilation at 0x1401AA320
    int size = m_nHeightGrid * m_nWidthGrid;
    for (int i = 0; i < size; ++i) {
        // Insert all objects from each grid cell into output vector
        vecOut.insert(vecOut.end(), m_pVecGridArray[i].begin(), m_pVecGridArray[i].end());
    }
}

template<typename T>
void UniformGrid<T>::Enumerate(int gridX, int gridY, std::vector<T>& vecOut) {
    // IDA: ?Enumerate@?$UniformGrid@PEAVCMover@@@@QEBA_KHHAEAV?$vector@PEAVCMover@@...
    // Verified from IDA decompilation at 0x1402A0820
    std::vector<T>* pGrid = FindGrid(gridX, gridY);
    if (!pGrid) {
        return;
    }
    // Insert all objects from the grid cell into output vector
    vecOut.insert(vecOut.end(), pGrid->begin(), pGrid->end());
}

template<typename T>
bool UniformGrid<T>::Insert(int gridX, int gridY, const T& obj) {
    // IDA: ?Insert@?$UniformGrid@PEAVCMover@@@@QEAA_NHHAEBQEAVCMover@@@Z
    // Verified from IDA decompilation at 0x14029BF40
    std::vector<T>* pGrid = FindGrid(gridX, gridY);
    if (!pGrid) {
        return false;
    }
    pGrid->push_back(obj);
    ++m_unSize;
    return true;
}

template<typename T>
bool UniformGrid<T>::Erase(int gridX, int gridY, const T& obj) {
    // IDA: ?Erase@?$UniformGrid@PEAVCMover@@@@QEAA_NHHAEBQEAVCMover@@@Z
    // Verified from IDA decompilation at 0x14029BFB0
    auto ret = FindElement(gridX, gridY, obj);
    bool bFound = std::get<0>(ret);
    if (!bFound) {
        return false;
    }
    
    std::vector<T>* pGrid = std::get<1>(ret);
    auto it = std::get<2>(ret);
    
    // Use "nomoving erase" pattern (swap with last element and pop)
    // This is more efficient than std::vector::erase for unordered data
    *it = pGrid->back();
    pGrid->pop_back();
    --m_unSize;
    return true;
}

// ============================================================================
// Range2DScanner Implementation
// ============================================================================

template<typename T>
Range2DScanner<T>::Range2DScanner()
    : m_beginPosX(0.0f)
    , m_beginPosY(0.0f)
    , m_lengthPerGrid(100.0f)
    , m_uniformGridPtr(nullptr)  // Will be initialized in SetSize
{
}

template<typename T>
Range2DScanner<T>::~Range2DScanner() {
}

template<typename T>
void Range2DScanner<T>::ScanGrid(float fx, float fy, int rangeX, int rangeY, std::vector<T>& vecOut) {
    // IDA 0x14029C080 - Full implementation
    // Algorithm:
    // 1. Convert position to grid coordinates
    // 2. Generate list of grid cells to scan
    // 3. For each cell, enumerate objects
    
    if (!m_uniformGridPtr) {
        return;
    }
    
    // Calculate grid coordinates from world position
    Coord srcCoord = CalcCoordFromPos(fx, fy);
    
    // Generate list of coordinates to scan
    std::vector<Coord> coordList;
    MakeCoordList(srcCoord.x, srcCoord.y, rangeX, rangeY, coordList);
    
    // Enumerate objects from each coordinate
    for (const Coord& coord : coordList) {
        m_uniformGridPtr->Enumerate(coord.x, coord.y, vecOut);
    }
}

template<typename T>
void Range2DScanner<T>::ScanGridAndSetObjCnt(float fx, float fy, int rangeX, int rangeY, std::vector<T>& vecOut, int nCount) {
    // IDA 0x14029C720
    // Similar to ScanGrid but also sets object count
    ScanGrid(fx, fy, rangeX, rangeY, vecOut);
    (void)nCount;  // TODO: Implement count logic
    GreenDamTan_log(__FILE__, __FUNCTION__, "ScanGridAndSetObjCnt");
}

template<typename T>
void Range2DScanner<T>::Enumerate(std::vector<T>& vecOut) {
    // IDA 0x1401A8DC0
    if (m_uniformGridPtr) {
        m_uniformGridPtr->EnumerateAll(vecOut);
    }
}

template<typename T>
bool Range2DScanner<T>::Insert(float dx, float dy, const T& obj) {
    // IDA 0x140299CA0
    if (!m_uniformGridPtr) return false;
    
    Coord coord = CalcCoordFromPos(dx, dy);
    return m_uniformGridPtr->Insert(coord.x, coord.y, obj);
}

template<typename T>
bool Range2DScanner<T>::Erase(float dx, float dy, const T& obj) {
    // IDA 0x140299D10
    if (!m_uniformGridPtr) return false;
    
    Coord coord = CalcCoordFromPos(dx, dy);
    return m_uniformGridPtr->Erase(coord.x, coord.y, obj);
}

template<typename T>
bool Range2DScanner<T>::Erase(const T& obj) {
    // IDA 0x1401A8D80
    // Erase without position - need to search all cells
    (void)obj;
    GreenDamTan_log(__FILE__, __FUNCTION__, "Erase (no pos) stub");
    return true;
}

template<typename T>
bool Range2DScanner<T>::Move(float oldX, float oldY, float newX, float newY, T obj) {
    // IDA 0x140299D80
    if (!m_uniformGridPtr) return false;
    
    Coord oldCoord = CalcCoordFromPos(oldX, oldY);
    Coord newCoord = CalcCoordFromPos(newX, newY);
    
    // If same cell, no need to move
    if (oldCoord.x == newCoord.x && oldCoord.y == newCoord.y) {
        return true;
    }
    
    // Remove from old cell, insert to new cell
    if (m_uniformGridPtr->Erase(oldCoord.x, oldCoord.y, obj)) {
        return m_uniformGridPtr->Insert(newCoord.x, newCoord.y, obj);
    }
    return false;
}

template<typename T>
typename Range2DScanner<T>::Coord Range2DScanner<T>::CalcCoordFromPos(float fx, float fy) {
    // IDA 0x14029A8C0 - Exact implementation
    // Formula: gridCoord = (worldPos - beginPos) / gridLength
    int gridX = static_cast<int>((fx - m_beginPosX) / m_lengthPerGrid);
    int gridY = static_cast<int>((fy - m_beginPosY) / m_lengthPerGrid);
    return Coord(gridX, gridY);
}

template<typename T>
void Range2DScanner<T>::SetSize(float beginX, float beginY, float endX, float endY, float gridLength) {
    // IDA: AREA_OBJECT::SetSize
    m_beginPosX = beginX;
    m_beginPosY = beginY;
    m_lengthPerGrid = gridLength;
    
    // Calculate grid dimensions
    int width = static_cast<int>((endX - beginX) / gridLength) + 1;
    int height = static_cast<int>((endY - beginY) / gridLength) + 1;
    
    // Create new UniformGrid with calculated dimensions
    m_uniformGridPtr = std::make_shared<UniformGrid<T>>(width, height);
}

template<typename T>
void Range2DScanner<T>::MakeCoordList(int centerX, int centerY, int rangeX, int rangeY, std::vector<Coord>& coordList) {
    // IDA 0x14029FA60 - Internal helper
    // Generate coordinates in a rectangle around center
    for (int dy = -rangeY; dy <= rangeY; ++dy) {
        for (int dx = -rangeX; dx <= rangeX; ++dx) {
            coordList.push_back(Coord(centerX + dx, centerY + dy));
        }
    }
}

template<typename T>
void Range2DScanner<T>::MakeCoordListFromRect(int minX, int minY, int maxX, int maxY, std::vector<Coord>& coordList) {
    // IDA 0x1402A0110 - Internal helper
    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            coordList.push_back(Coord(x, y));
        }
    }
}

// ============================================================================
// AREA_OBJECT Implementation
// ============================================================================

AREA_OBJECT::~AREA_OBJECT() {
    // Clean up dynamically allocated scanners
    delete playerScanner;
    playerScanner = nullptr;

    delete npcScanner;
    npcScanner = nullptr;

    delete etcScanner;
    etcScanner = nullptr;
}

void AREA_OBJECT::SetSize(float beginX, float beginY, float endX, float endY, float gridLength) {
    // IDA: Initialize all scanners with dimensions
    // Create scanners if they don't exist
    if (!playerScanner) {
        playerScanner = new Range2DScanner<CMover*>();
    }
    playerScanner->SetSize(beginX, beginY, endX, endY, gridLength);

    if (!npcScanner) {
        npcScanner = new Range2DScanner<CMover*>();
    }
    npcScanner->SetSize(beginX, beginY, endX, endY, gridLength);

    if (!etcScanner) {
        etcScanner = new Range2DScanner<CMover*>();
    }
    etcScanner->SetSize(beginX, beginY, endX, endY, gridLength);
}

// Explicit template instantiation for CMover*
template class Range2DScanner<CMover*>;
template class UniformGrid<CMover*>;

// ============================================================================
// UniformGrid Private Helper Functions
// ============================================================================

template<typename T>
std::vector<T>* UniformGrid<T>::FindGrid(int x, int y) const {
    // IDA: ?FindGrid@?$UniformGrid@PEAVCMover@@@@AEBAPEAV?$vector@PEAVCMover@@...@@HH@Z
    // Verified from IDA decompilation at 0x14029FAF0
    if (x < 0 || x >= m_nWidthGrid) {
        return nullptr;
    }
    if (y >= 0 && y < m_nHeightGrid) {
        return &m_pVecGridArray[ConvertPosToIndex(x, y)];
    }
    return nullptr;
}

template<typename T>
std::tuple<bool, std::vector<T>*, typename std::vector<T>::iterator>
UniformGrid<T>::FindElement(int x, int y, const T& obj) const {
    // IDA: ?FindElement@?$UniformGrid@PEAVCMover@@@@AEBA?AV?$tuple@...
    // Verified from IDA decompilation at 0x14029FB70
    std::vector<T>* pGrid = FindGrid(x, y);
    bool bFound = false;
    typename std::vector<T>::iterator it;
    
    if (pGrid) {
        // Search for the object in the grid cell
        it = std::find(pGrid->begin(), pGrid->end(), obj);
        if (it != pGrid->end()) {
            bFound = true;
        }
    }
    
    return std::make_tuple(bFound, pGrid, it);
}

template<typename T>
int UniformGrid<T>::ConvertPosToIndex(int x, int y) const {
    // IDA: ?ConvertPosToIndex@?$UniformGrid@PEAVCMover@@@@AEBAHHH@Z
    // Verified from IDA decompilation at 0x1402A01C0
    return x + m_nWidthGrid * y;
}
