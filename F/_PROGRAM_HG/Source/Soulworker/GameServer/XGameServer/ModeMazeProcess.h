// ModeMazeProcess.h - CModeMazeProcess class declaration
// Restored from GameServer.exe IDA decompilation

#pragma once

#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"

// Forward declarations
class XPacket;
struct PS_MODE_MAZE_MATCHING_ENTER_REQ;

// ============================================================================
// CModeMazeProcess - Mode Maze packet process handler
// IDA: Inherits from TXProcess<CUser>
//
// Function: Handles mode maze related packet processing including:
// - Matching enter/exit requests
// - Roguelike mode requests
// ============================================================================
class CModeMazeProcess : public TXProcess<CUser> {
public:
    // === Constructor/Destructor ===
    // IDA: ??0CModeMazeProcess@@QEAA@XZ (0x140514680)
    CModeMazeProcess();

    // IDA: ??1CModeMazeProcess@@UEAA@XZ (0x140514740)
    virtual ~CModeMazeProcess();

    // === Packet Processing ===
    // IDA: ?Parse@CModeMazeProcess@@UEAA_NAEAVXPacket@@@Z (0x140514770)
    virtual bool Parse(XPacket& xPacket);

    // === Request Handlers ===
    // IDA: ?ReqModeMazeMatchingEnter@CModeMazeProcess@@QEAA_NAEAVXPacket@@@Z (0x140514820)
    bool ReqModeMazeMatchingEnter(XPacket& xPacket);

    // IDA: ?ReqModeMazeMatchingExit@CModeMazeProcess@@QEAA_NAEAVXPacket@@@Z (0x140515090)
    bool ReqModeMazeMatchingExit(XPacket& xPacket);

    // IDA: ?CheckModeMazeMatchingEnterUser@CModeMazeProcess@@QEAA_NPEAVCUser@@GAEAH1@Z (0x1405153b0)
    bool CheckModeMazeMatchingEnterUser(CUser* pUser, unsigned short wModeType, int& nResult, int& nErrorCode);

    // IDA: ?ReqModeMazeRoguelikeEnter@CModeMazeProcess@@QEAA_NAEAVXPacket@@@Z (0x1405159b0)
    bool ReqModeMazeRoguelikeEnter(XPacket& xPacket);

    // IDA: ?ReqModeMazeRoguelikeSelectPocket@CModeMazeProcess@@QEAA_NAEAVXPacket@@@Z (0x140516380)
    bool ReqModeMazeRoguelikeSelectPocket(XPacket& xPacket);

    // IDA: ?ReqModeMazeRoguelikeShopBuy@CModeMazeProcess@@QEAA_NAEAVXPacket@@@Z (0x140516650)
    bool ReqModeMazeRoguelikeShopBuy(XPacket& xPacket);
};
