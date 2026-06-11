// XMaze.h
// XMaze - Maze area class
// IDA decompilation from GameServer.exe

#pragma once

#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include <cstdint>

// Forward declarations
class IVScriptInstance;
class DohHavokNavMeshInstance;

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
