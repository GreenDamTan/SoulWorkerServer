// MoveProcess.h
// Movement system packet handler for GameServer
// Reconstructed from IDA decompilation

#pragma once

#include "../../XCore/XServer/TXProcess.h"

class CUser;
class XPacket;

class XMoveProcess : public TXProcess<CUser>
{
public:
    XMoveProcess();
    virtual ~XMoveProcess();

    // Main packet parser
    virtual bool Parse(XPacket& xPacket) override;

    // Movement request handlers
    bool ReqMove(XPacket& xPacket);
    bool ReqStop(XPacket& xPacket);
    bool ReqJump(XPacket& xPacket);
    bool ReqBattle(XPacket& xPacket);
    bool ReqMotion(XPacket& xPacket);
    bool ReqMotionDelta(XPacket& xPacket);
    bool ReqTransportTake(XPacket& xPacket);
    bool ReqTransportOff(XPacket& xPacket);
    bool ReqMoveRotation(XPacket& xPacket);
    bool ReqPosition(XPacket& xPacket);
    bool ReqLoopMotionStart(XPacket& xPacket);
    bool ReqLoopMotionEnd(XPacket& xPacket);
    bool ReqAttachedEnd(XPacket& xPacket);
    bool ReqGroundStatus(XPacket& xPacket);
    bool ReqJumpQuickDown(XPacket& xPacket);
};
