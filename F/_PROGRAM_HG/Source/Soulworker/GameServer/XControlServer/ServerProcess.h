// ServerProcess.h - ControlServer
// 对齐 IDA ControlServer.exe: CServerProcess 包处理器

#pragma once

#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "CServer.h"
#include <memory>

// 前向声明
class XControlServer;

// 对齐 IDA ControlServer.exe: CServerProcess
class CServerProcess : public TXProcess<CServer> {
public:
    CServerProcess() {
        SetCmd(0xF2);  // 对齐 IDA: main command 0xF2
        SetName("CServerProcess");
    }
    virtual ~CServerProcess() = default;

    // 对齐 IDA 0x14003CAC0: Parse - 包解析入口
    bool Parse(XPacket& xPacket) override;

protected:
    // 对齐 IDA ControlServer.exe 的 sub commands
    // 对齐 IDA 0x14003CD60: ReqCreateServer (sub 0x01)
    bool ReqCreateServer(XPacket& xPacket);

    // 对齐 IDA 0x14003CDE0: ReqUpdateServerInfo (sub 0x03)
    bool ReqUpdateServerInfo(XPacket& xPacket);

    // 对齐 IDA 0x14003CE20: ReqChangeChannel (sub 0x10)
    bool ReqChangeChannel(XPacket& xPacket);

    // 对齐 IDA 0x14003D080: ReqCreateMaze (sub 0x21)
    bool ReqCreateMaze(XPacket& xPacket);

    // 对齐 IDA 0x14003D1E0: ResCreateMaze (sub 0x22)
    bool ResCreateMaze(XPacket& xPacket);

    // 对齐 IDA 0x14003D310: SyncMaze (sub 0x23)
    bool SyncMaze(XPacket& xPacket);

    // 对齐 IDA 0x14003D5D0: SyncRemoveMaze (sub 0x25)
    bool SyncRemoveMaze(XPacket& xPacket);

    // 对齐 IDA 0x14003D720: SyncUpdateMaze (sub 0x26)
    bool SyncUpdateMaze(XPacket& xPacket);

    // 对齐 IDA 0x14003D9E0: ReqGoBackMaze (sub 0x27)
    bool ReqGoBackMaze(XPacket& xPacket);

    // 对齐 IDA 0x14003DC90: ReqCreateMap (sub 0x30)
    bool ReqCreateMap(XPacket& xPacket);

    // 对齐 IDA 0x14003DE20: ReqEnterMap (sub 0x31)
    bool ReqEnterMap(XPacket& xPacket);

    // 对齐 IDA 0x14003DF40: ReqCheckPartyInMaze (sub 0x32)
    bool ReqCheckPartyInMaze(XPacket& xPacket);

    // 对齐 IDA 0x14003E070: SyncUsersInfo (sub 0x33)
    bool SyncUsersInfo(XPacket& xPacket);

    // 对齐 IDA 0x14003E190: SyncUserPartyInfo (sub 0x36)
    bool SyncUserPartyInfo(XPacket& xPacket);

    // 对齐 IDA 0x14003E570: SyncLogicThreadCount (sub 0x37)
    bool SyncLogicThreadCount(XPacket& xPacket);

    // 对齐 IDA 0x14003E5C0: ReqCheckEnterMaze (sub 0x38)
    bool ReqCheckEnterMaze(XPacket& xPacket);

    // 对齐 IDA 0x14003E7D0: ReqDisconnectUserSync (sub 0x39)
    bool ReqDisconnectUserSync(XPacket& xPacket);

    // 对齐 IDA 0x14003E9A0: SyncMaxMazeID (sub 0x40)
    bool SyncMaxMazeID(XPacket& xPacket);

    // 对齐 IDA 0x14003EA10: ReqReEnterMap (sub 0x41)
    bool ReqReEnterMap(XPacket& xPacket);

    // 对齐 IDA 0x14003EB30: ReqRemoveUserSync (sub 0x42)
    bool ReqRemoveUserSync(XPacket& xPacket);

    // 对齐 IDA 0x14003EC60: ReqCreateMyRoom (sub 0x50)
    bool ReqCreateMyRoom(XPacket& xPacket);

    // 对齐 IDA 0x14003EE60: DeleteMyRoomReq (sub 0x51)
    bool DeleteMyRoomReq(XPacket& xPacket);

    // 对齐 IDA 0x14003EF20: DeleteMyRoomRes (sub 0x52)
    bool DeleteMyRoomRes(XPacket& xPacket);

    // 对齐 IDA 0x14003EFC0: ReqGoBackMyRoom (sub 0x53)
    bool ReqGoBackMyRoom(XPacket& xPacket);

    // 对齐 IDA 0x14003F0C0: ReqEnterMyRoom (sub 0x54)
    bool ReqEnterMyRoom(XPacket& xPacket);

    // 对齐 IDA 0x14003F290: ResEnterMyRoom (sub 0x55)
    bool ResEnterMyRoom(XPacket& xPacket);

    // 对齐 IDA: ResCreateMatchingMazeFromGame (sub 0x41/0x42)
    bool ResCreateMatchingMazeFromGame(XPacket& xPacket, E_PARTY_GROUP_TYPE eType);

    // 对齐 IDA: ReqCreateMatchingMazeFromCommunity (sub 0x43)
    bool ReqCreateMatchingMazeFromCommunity(XPacket& xPacket);

    // 对齐 IDA: ReqCreateMatchingModeMazeFromCommunity (sub 0x49)
    bool ReqCreateMatchingModeMazeFromCommunity(XPacket& xPacket);

    // 对齐 IDA: ReqMyRoomEnterReq (sub 0x50)
    bool ReqMyRoomEnterReq(XPacket& xPacket);

    // 对齐 IDA: ReqMyRoomEnterRes (sub 0x51)
    bool ReqMyRoomEnterRes(XPacket& xPacket);

    // 对齐 IDA: ReqMyRoomCreate (sub 0x52)
    bool ReqMyRoomCreate(XPacket& xPacket);

    // 对齐 IDA: ReqMyRoomDelete (sub 0x53)
    bool ReqMyRoomDelete(XPacket& xPacket);

    // 对齐 IDA: ResMyRoomDelete (sub 0x54)
    bool ResMyRoomDelete(XPacket& xPacket);

    // 对齐 IDA: EnterOtherMap_cheat (sub 0x60)
    bool EnterOtherMap_cheat(XPacket& xPacket);

    // 对齐 IDA: PartyMazeSync (sub 0x70)
    bool PartyMazeSync(XPacket& xPacket);

    // 对齐 IDA: ForceMazeSync (sub 0x71)
    bool ForceMazeSync(XPacket& xPacket);

    // 对齐 IDA: ReqPostSend (sub 0x80)
    bool ReqPostSend(XPacket& xPacket);

    // 对齐 IDA: ReqFindUser (sub 0x81)
    bool ReqFindUser(XPacket& xPacket);

    // 对齐 IDA: ResCreateModeMaze (sub 0x90)
    bool ResCreateModeMaze(XPacket& xPacket);

    // 对齐 IDA: ReqUpdateRouletteEvent (sub 0xA0)
    bool ReqUpdateRouletteEvent(XPacket& xPacket);
};
