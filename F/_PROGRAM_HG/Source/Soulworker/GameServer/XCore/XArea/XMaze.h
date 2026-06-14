// XMaze.h
// XMaze - Maze area class
// IDA decompilation from GameServer.exe

#pragma once

#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"  // for hkvVec3
#include <cstdint>
#include <vector>

// Forward declarations
class IVScriptInstance;
class DohHavokNavMeshInstance;
struct tagPATHFIND_POINT;

/**
 * @brief XMaze - Maze area class
 *
 * Extends XArea with maze-specific functionality like
 * script instances, nav mesh, and event object resources.
 */
class XMaze : public XArea {
public:
    // Constructor
    XMaze();

    // Destructor
    virtual ~XMaze();

    // Get script instance for this maze
    // IDA: ?GetArea@XMaze@@QEAAPEAVIVScriptInstance@@XZ
    IVScriptInstance* GetArea();

    // Get nav mesh instance
    // IDA: ?GetNavMeshInstance@XMaze@@QEAAPEAVDohHavokNavMeshInstance@@XZ
    DohHavokNavMeshInstance* GetNavMeshInstance();

    // Add helper NPC
    // IDA: ?AddHelper@XMaze@@QEAA_NPEAVCUser@@@Z
    bool AddHelper(class CUser* pUser);

    // IDA: ?CheckCanDirectMove2@XMaze@@SAHPEAVDohHavokNavMeshInstance@@AEAVhkvVec3@@1MHH@Z @ 0x14032AAD0
    // Static function to check if direct move is possible
    static int CheckCanDirectMove2(DohHavokNavMeshInstance* pNavMesh, hkvVec3* vStartPos,
                                   hkvVec3* vDestPos, float fRadius, int bFlying, int bDontCareCurve);

    // IDA 0x14032EE00 - ScanGridOrigin
    // IDA: ?ScanGridOrigin@XMaze@@UEAAXMMEHKAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z
    void ScanGridOrigin(float dx, float dy, unsigned char byNation, int sectorRange, unsigned int dwOptions, std::vector<CMover*>& vecOut) override;

private:
    IVScriptInstance* m_pScriptInstance;
    DohHavokNavMeshInstance* m_pNavMeshInstance;
};

/**
 * @brief IVScriptInstance - Script instance interface
 * 
 * Interface for script instances that can execute Lua scripts.
 */
class IVScriptInstance {
public:
    virtual ~IVScriptInstance() {}

    // Execute script callback
    virtual void ExecuteCallback(int nKey, int nParam) = 0;

    // Execute function with arguments (variadic)
    virtual void ExecuteFunctionArg(const char* szFuncName, const char* szSig, ...) = 0;

    // Get script variable
    virtual int GetVariable(const char* szName) = 0;

    // Set script variable
    virtual void SetVariable(const char* szName, int nValue) = 0;
};
