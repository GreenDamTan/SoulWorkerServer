// Range2DScanner.h
// 2D Grid-based spatial partitioning scanner
// Reconstructed from IDA decompilation

#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <tuple>
#include <algorithm>

// Forward declarations
class CMover;

// ============================================================================
// UniformGrid - Internal grid storage
// ============================================================================
template<typename T>
class UniformGrid {
public:
    // Constructor
    // IDA: ??0?$UniformGrid@PEAVCMover@@@@QEAA@HH@Z
    UniformGrid(int width, int height);
    ~UniformGrid();
    
    // EnumerateAll - Enumerate all objects in the grid
    // IDA: ?EnumerateAll@?$UniformGrid@PEAVCMover@@@@QEAAXAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z
    void EnumerateAll(std::vector<T>& vecOut);
    
    // Enumerate - Enumerate objects at specific grid cell
    // IDA: ?Enumerate@?$UniformGrid@PEAVCMover@@@@QEAAXHHAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z
    void Enumerate(int gridX, int gridY, std::vector<T>& vecOut);
    
    // Insert - Insert object at grid position
    // IDA: ?Insert@?$UniformGrid@PEAVCMover@@@@QEAA_NHHAEBQEAVCMover@@@Z
    bool Insert(int gridX, int gridY, const T& obj);
    
    // Erase - Remove object from grid position
    // IDA: ?Erase@?$UniformGrid@PEAVCMover@@@@QEAA_NHHAEBQEAVCMover@@@Z
    bool Erase(int gridX, int gridY, const T& obj);
    
private:
    // FindGrid - Find grid cell at coordinates
    // IDA: ?FindGrid@?$UniformGrid@PEAVCMover@@@@AEBAPEAV?$vector@PEAVCMover@@...@@HH@Z
    std::vector<T>* FindGrid(int x, int y) const;
    
    // FindElement - Find element in grid cell
    // IDA: ?FindElement@?$UniformGrid@PEAVCMover@@@@AEBA?AV?$tuple@...
    std::tuple<bool, std::vector<T>*, typename std::vector<T>::iterator> 
    FindElement(int x, int y, const T& obj) const;
    
    // ConvertPosToIndex - Convert 2D grid position to 1D array index
    // IDA: ?ConvertPosToIndex@?$UniformGrid@PEAVCMover@@@@AEBAHHH@Z
    int ConvertPosToIndex(int x, int y) const;
    
    // === Member variables (from IDA struct analysis) ===
    std::vector<T>* m_pVecGridArray;  // Array of vectors for each grid cell
    int m_nWidthGrid;                  // Grid width (number of cells in X)
    int m_nHeightGrid;                 // Grid height (number of cells in Y)
    size_t m_unSize;                   // Total number of objects in grid
};

// ============================================================================
// Range2DScanner - 2D Grid-based spatial scanner
// IDA: ?ScanGrid@?$Range2DScanner@PEAVCMover@@@@
// ============================================================================
template<typename T>
class Range2DScanner {
public:
    // Coord - Grid coordinate
    // IDA: struct Coord at 0x14029B0A0
    struct Coord {
        int x;
        int y;
        
        Coord() : x(0), y(0) {}
        Coord(int _x, int _y) : x(_x), y(_y) {}
    };
    
    Range2DScanner();
    ~Range2DScanner();
    
    // === IDA-decompiled methods ===
    
    // ScanGrid - Scan grid around position and collect objects
    // IDA 0x14029C080 - ?ScanGrid@?$Range2DScanner@PEAVCMover@@@@QEAAXMMHHAEAV?$vector@PEAVCMover@@...
    // Algorithm:
    // 1. Convert position (fx, fy) to grid coordinates using CalcCoordFromPos
    // 2. Generate list of grid cells to scan using MakeCoordList
    // 3. For each cell, enumerate objects using UniformGrid::Enumerate
    void ScanGrid(float fx, float fy, int rangeX, int rangeY, std::vector<T>& vecOut);
    
    // ScanGridAndSetObjCnt - Scan grid and set object count
    // IDA 0x14029C720
    void ScanGridAndSetObjCnt(float fx, float fy, int rangeX, int rangeY, std::vector<T>& vecOut, int nCount);
    
    // Enumerate - Enumerate all objects in the scanner
    // IDA 0x1401A8DC0 - ?Enumerate@?$Range2DScanner@PEAVCMover@@@@QEAAXAEAV?$vector@PEAVCMover@@...
    // Implementation: UniformGrid::EnumerateAll(m_uniformGridPtr.get(), vec)
    void Enumerate(std::vector<T>& vecOut);
    
    // Insert - Insert object at position
    // IDA 0x140299CA0 - ?Insert@?$Range2DScanner@PEAVCMover@@@@QEAA_NMMAEBQEAVCMover@@@Z
    // Parameters: dx, dy - grid coordinates, obj - object to insert
    bool Insert(float dx, float dy, const T& obj);
    
    // Erase - Remove object from position
    // IDA 0x140299D10 - ?Erase@?$Range2DScanner@PEAVCMover@@@@QEAA_NMMAEBQEAVCMover@@@Z
    bool Erase(float dx, float dy, const T& obj);
    
    // Erase - Remove object (without position)
    // IDA 0x1401A8D80 - ?Erase@?$Range2DScanner@PEAVCMover@@@@QEAA_NAEBQEAVCMover@@@Z
    bool Erase(const T& obj);
    
    // Move - Move object from old position to new position
    // IDA 0x140299D80 - ?Move@?$Range2DScanner@PEAVCMover@@@@QEAA_NMMMMPEAVCMover@@@Z
    // Parameters: oldX, oldY, newX, newY, obj
    bool Move(float oldX, float oldY, float newX, float newY, T obj);
    
    // CalcCoordFromPos - Calculate grid coordinates from world position
    // IDA 0x14029A8C0 - ?CalcCoordFromPos@?$Range2DScanner@PEAVCMover@@@@QEAA?AUCoord@1@MM@Z
    // Formula:
    //   gridX = (fx - m_beginPosX) / m_lengthPerGrid
    //   gridY = (fy - m_beginPosY) / m_lengthPerGrid
    Coord CalcCoordFromPos(float fx, float fy);
    
    // SetSize - Set scanner dimensions
    // IDA: AREA_OBJECT::SetSize
    void SetSize(float beginX, float beginY, float endX, float endY, float gridLength);
    
    // === Accessors ===
    float GetBeginPosX() const { return m_beginPosX; }
    float GetBeginPosY() const { return m_beginPosY; }
    float GetLengthPerGrid() const { return m_lengthPerGrid; }
    
private:
    // MakeCoordList - Generate list of grid coordinates to scan
    // IDA 0x14029FA60 - ?MakeCoordList@?$Range2DScanner@PEAVCMover@@@@AEAAXHHHHAEAV?$vector@UCoord...
    void MakeCoordList(int centerX, int centerY, int rangeX, int rangeY, std::vector<Coord>& coordList);
    
    // MakeCoordListFromRect - Generate coordinate list from rectangle
    // IDA 0x1402A0110
    void MakeCoordListFromRect(int minX, int minY, int maxX, int maxY, std::vector<Coord>& coordList);
    
    // === Member variables (from IDA struct analysis) ===
    float m_beginPosX;                              // World position X of grid origin
    float m_beginPosY;                              // World position Y of grid origin
    float m_lengthPerGrid;                          // Size of each grid cell
    std::shared_ptr<UniformGrid<T>> m_uniformGridPtr;  // Shared pointer to uniform grid
};

// ============================================================================
// AREA_OBJECT - Object scanner container for XDistrict
// IDA: struct AREA_OBJECT (size: 24 bytes)
// ============================================================================
struct AREA_OBJECT {
    Range2DScanner<CMover*>* playerScanner;  // offset 0x00, 8 bytes - Scanner for player objects
    Range2DScanner<CMover*>* npcScanner;     // offset 0x08, 8 bytes - Scanner for NPC objects
    Range2DScanner<CMover*>* etcScanner;     // offset 0x10, 8 bytes - Scanner for other objects

    AREA_OBJECT()
        : playerScanner(nullptr)
        , npcScanner(nullptr)
        , etcScanner(nullptr)
    {}

    ~AREA_OBJECT();

    // SetSize - Initialize all scanners with dimensions
    // IDA: AREA_OBJECT::SetSize
    void SetSize(float beginX, float beginY, float endX, float endY, float gridLength);
};
