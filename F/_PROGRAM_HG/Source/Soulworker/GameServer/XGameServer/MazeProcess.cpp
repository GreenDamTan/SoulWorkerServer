// MazeProcess.cpp - CMazeProcess class implementation
// Restored from GameServer.exe IDA decompilation
// Constructor: 0x14050A080
// Destructor: 0x14050A140

#include "MazeProcess.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/Maze.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XCore/XServer/Packet.h"
#include "Soulworker/GameServer/XCore/XServer/Client.h"
#include <cstdio>
#include <cstring>

// ============================================================================
// CMazeProcess Constructor
// IDA: 0x14050A080
// ============================================================================
CMazeProcess::CMazeProcess()
    : TXProcess<CUser>()
{
    // Base class constructor handles initialization
}

// ============================================================================
// CMazeProcess Destructor
// IDA: 0x14050A140
// ============================================================================
CMazeProcess::~CMazeProcess() {
    // Base class destructor handles cleanup
}

// ============================================================================
// Parse - Main packet dispatcher
// IDA: 0x14050A170
// ============================================================================
bool CMazeProcess::Parse(XPacket* pPacket) {
    if (!pPacket) {
        return false;
    }

    unsigned char bySubCmd = pPacket->GetSubCmd();

    switch (bySubCmd) {
        case 0x22:  // '"' - Complete maze
            ReqCompleteMaze(pPacket);
            break;
        case 0x23:  // '#' - Exit maze
            ReqExitMaze(pPacket);
            break;
        case 0x25:  // '%' - Complete maze start game
            ReqCompleteMazeStartGame(pPacket);
            break;
        case 0x31:  // '1' - Event spawn box
            ReqEventSpawnBox(pPacket);
            break;
        case 0x33:  // '3' - Event scene directing
            ReqEventSceneDirecting(pPacket);
            break;
        case 0x35:  // '5' - Check event spawn box
            ReqCheckEventSpawnBox(pPacket);
            break;
        case 0x43:  // 'C' - Enter maze
            ReqEnterMaze(pPacket);
            break;
        case 0x48:  // 'H' - Maze enter party
            ReqMazeEnterParty(pPacket);
            break;
        case 0x4C:  // 'L' - Maze enter force
            ReqMazeEnterForce(pPacket);
            break;
        case 0x61:  // 'a' - NPC talk end
            ReqNpcTalkEnd(pPacket);
            break;
        case 0x62:  // 'b' - Operation end
            ReqOperationEnd(pPacket);
            break;
        case 0x65:  // 'e' - Quest move check
            ReqQuestMoveCheck(pPacket);
            break;
        case 0x77:  // 'w' - Interaction click
            ReqInteractionClick(pPacket);
            break;
        case 0x7E:  // '~' - Lua function
            ReqLuaFunction(pPacket);
            break;
        default:
            // Unknown command - ignore
            break;
    }

    return true;
}

// ============================================================================
// ReqEnterMaze - Request to enter dungeon
// IDA: 0x14050A370
// ============================================================================
bool CMazeProcess::ReqEnterMaze(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer from client
    // 2. Parse ST_ENTER_MAZE from packet
    // 3. Send DB request to enter maze

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // Parse enter maze request
    ST_ENTER_MAZE stEnterMaze;
    *pPacket >> stEnterMaze;

    // Get server ID
    XGameServer* pServer = XGameServer::Instance();
    unsigned int dwServerID = pServer->GetServerID();

    // Create DB packet
    XSendDBPacket xSendDBPacket(static_cast<XActor*>(pUser), 3, 0x41);
    xSendDBPacket << stEnterMaze;

    // Send to DB
    pServer->SendDBGame(&xSendDBPacket);

    return true;
}

// ============================================================================
// ReqCompleteMaze - Request to complete dungeon
// IDA: 0x14050A4E0
// ============================================================================
bool CMazeProcess::ReqCompleteMaze(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer
    // 2. Parse completion request
    // 3. Notify maze of completion

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // TODO: Parse PS_REQ_COMPLETE_MAZE structure
    // TODO: Find user's current maze and call CompleteMaze

    return true;
}

// ============================================================================
// ReqExitMaze - Request to exit dungeon
// IDA: 0x14050AA10
// ============================================================================
bool CMazeProcess::ReqExitMaze(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer from client
    // 2. Parse ST_CHANGE_WORLD and bAuto from packet
    // 3. Use CLogicThreadManager::DoJob to process exit in correct thread

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // Parse exit request
    ST_CHANGE_WORLD stChangeWorld;
    *pPacket >> stChangeWorld;

    bool bAuto = false;
    pPacket->XParse >> bAuto;

    // Get current map instance ID
    UXMapID result;
    UXMapID* pMapID = pUser->GetValidMapInsID(&result);
    long long nActionMapID = pMapID->nMapID;

    // TODO: Lambda capture and CLogicThreadManager::DoJob
    // The original uses lambdas to queue work to the correct thread:
    // - First lambda: handles exit logic
    // - Second lambda: cleanup after exit

    // For now, implement simplified version
    XArea* pArea = pUser->GetArea();
    if (!pArea) {
        return false;
    }

    // TODO: Call proper exit logic through thread manager
    // CLogicThreadManager::Instance()->DoJob(mapInsID, lambda);

    return true;
}

// ============================================================================
// ReqCompleteMazeStartGame - Request to complete maze and start game
// IDA: 0x14050BEE0
// ============================================================================
bool CMazeProcess::ReqCompleteMazeStartGame(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer
    // 2. Parse start game request
    // 3. Notify maze to start game

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // TODO: Parse request structure
    // TODO: Find maze and call start game

    return true;
}

// ============================================================================
// ReqEventSpawnBox - Request event spawn box
// IDA: 0x14050C1D0
// ============================================================================
bool CMazeProcess::ReqEventSpawnBox(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer
    // 2. Parse spawn box ID
    // 3. Execute spawn box in maze

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // TODO: Parse PS_REQ_EVENT_SPAWN_BOX
    // TODO: Find maze and call ExcuteSpawnBox

    return true;
}

// ============================================================================
// ReqCheckEventSpawnBox - Request to check event spawn box
// IDA: 0x14050C490
// ============================================================================
bool CMazeProcess::ReqCheckEventSpawnBox(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer
    // 2. Parse check spawn box request
    // 3. Execute check in maze

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // TODO: Parse request and execute check

    return true;
}

// ============================================================================
// ReqEventSceneDirecting - Request event scene directing
// IDA: 0x14050C7B0
// ============================================================================
bool CMazeProcess::ReqEventSceneDirecting(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer
    // 2. Parse scene directing request
    // 3. Process scene direction

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // TODO: Parse request and process scene

    return true;
}

// ============================================================================
// ReqInteractionClick - Request interaction click
// IDA: 0x14050CBE0
// ============================================================================
bool CMazeProcess::ReqInteractionClick(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer
    // 2. Parse interaction box ID
    // 3. Call XMaze::ClickInteractionBox

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // TODO: Parse PS_REQ_INTERACTION_CLICK
    // TODO: Get maze and call ClickInteractionBox

    return true;
}

// ============================================================================
// ReqLuaFunction - Request Lua function execution
// IDA: 0x14050CF70
// ============================================================================
bool CMazeProcess::ReqLuaFunction(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer
    // 2. Parse Lua function ID
    // 3. Execute Lua function in maze

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // TODO: Parse PS_REQ_LUA_FUNCTION
    // TODO: Get maze and call RunLuaFunction

    return true;
}

// ============================================================================
// ReqNpcTalkEnd - Request NPC talk end
// IDA: 0x14050D280
// ============================================================================
bool CMazeProcess::ReqNpcTalkEnd(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer
    // 2. Parse NPC talk end request
    // 3. Process NPC talk end in maze

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // TODO: Parse request and process

    return true;
}

// ============================================================================
// ReqOperationEnd - Request operation end
// IDA: 0x14050D560
// ============================================================================
bool CMazeProcess::ReqOperationEnd(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer
    // 2. Parse operation end request
    // 3. Process operation end in maze

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // TODO: Parse request and process

    return true;
}

// ============================================================================
// ReqQuestMoveCheck - Request quest move check
// IDA: 0x14050D840
// ============================================================================
bool CMazeProcess::ReqQuestMoveCheck(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer
    // 2. Parse quest move check request
    // 3. Run quest move check in maze

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // TODO: Parse request and process

    return true;
}

// ============================================================================
// ReqMazeEnterParty - Request maze enter for party
// IDA: 0x14050DC80
// ============================================================================
bool CMazeProcess::ReqMazeEnterParty(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer
    // 2. Parse party enter request
    // 3. Process party enter to maze

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // TODO: Parse request and process

    return true;
}

// ============================================================================
// ReqMazeEnterForce - Request maze enter for force
// IDA: 0x14050E2B0
// ============================================================================
bool CMazeProcess::ReqMazeEnterForce(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer
    // 2. Parse force enter request
    // 3. Process force enter to maze

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // TODO: Parse request and process

    return true;
}

// ============================================================================
// CModeMazeProcess Implementation
// ============================================================================

// Constructor - IDA: 0x140514680
CModeMazeProcess::CModeMazeProcess()
    : TXProcess<CUser>()
{
}

// Destructor - IDA: 0x140514740
CModeMazeProcess::~CModeMazeProcess() {
}

// Parse - IDA: 0x140514770
bool CModeMazeProcess::Parse(XPacket* pPacket) {
    if (!pPacket) {
        return false;
    }

    unsigned char bySubCmd = pPacket->GetSubCmd();

    switch (bySubCmd) {
        case 0x2A:  // '*' - Mode maze matching enter
            ReqModeMazeMatchingEnter(pPacket);
            break;
        case 0x2B:  // '+' - Mode maze matching exit
            ReqModeMazeMatchingExit(pPacket);
            break;
        case 0x30:  // '0' - Mode maze roguelike enter
            ReqModeMazeRoguelikeEnter(pPacket);
            break;
        case 0x40:  // '@' - Mode maze roguelike select pocket
            ReqModeMazeRoguelikeSelectPocket(pPacket);
            break;
        case 0x50:  // 'P' - Mode maze roguelike shop buy
            ReqModeMazeRoguelikeShopBuy(pPacket);
            break;
        default:
            break;
    }

    return true;
}

// ReqModeMazeMatchingEnter - IDA: 0x140514820
bool CModeMazeProcess::ReqModeMazeMatchingEnter(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer
    // 2. Parse matching enter request (PS_MODE_MAZE_MATCHING_ENTER_REQ)
    // 3. Check user can enter mode maze
    // 4. Register for matching or enter directly

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // TODO: Parse PS_MODE_MAZE_MATCHING_ENTER_REQ
    // TODO: CheckModeMazeMatchingEnterUser validation
    // TODO: Register for mode maze matching

    return true;
}

// ReqModeMazeMatchingExit - IDA: 0x140515090
bool CModeMazeProcess::ReqModeMazeMatchingExit(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer
    // 2. Parse matching exit request
    // 3. Remove from matching queue

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // TODO: Parse request and remove from queue

    return true;
}

// CheckModeMazeMatchingEnterUser - IDA: 0x1405153B0
bool CModeMazeProcess::CheckModeMazeMatchingEnterUser(CUser* pUser, 
    unsigned short nMazeID, int* pnResult1, int* pnResult2) {
    // IDA Decompiled Logic:
    // 1. Validate user pointer
    // 2. Check maze ID validity
    // 3. Check user level requirements
    // 4. Check enter limit count
    // 5. Return validation results

    if (!pUser) {
        return false;
    }

    // TODO: Implement full validation logic from IDA
    // - Check maze table exists
    // - Check user level >= maze requirement
    // - Check daily/weekly enter limit
    // - Check required items

    return true;
}

// ReqModeMazeRoguelikeEnter - IDA: 0x1405159B0
bool CModeMazeProcess::ReqModeMazeRoguelikeEnter(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer
    // 2. Parse roguelike enter request
    // 3. Initialize roguelike maze state

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // TODO: Parse request and initialize roguelike

    return true;
}

// ReqModeMazeRoguelikeSelectPocket - IDA: 0x140516380
bool CModeMazeProcess::ReqModeMazeRoguelikeSelectPocket(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer
    // 2. Parse pocket selection
    // 3. Apply selected pocket rewards

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // TODO: Parse request and process pocket selection

    return true;
}

// ReqModeMazeRoguelikeShopBuy - IDA: 0x140516650
bool CModeMazeProcess::ReqModeMazeRoguelikeShopBuy(XPacket* pPacket) {
    // IDA Decompiled Logic:
    // 1. Get CUser pointer
    // 2. Parse shop buy request
    // 3. Process purchase in roguelike shop

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }

    // TODO: Parse request and process shop purchase

    return true;
}
