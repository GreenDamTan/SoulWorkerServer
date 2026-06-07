#pragma once

#include "Soulworker/GameServer/XCore/XServer/TXProcess.h"

class CUser;
class XPacket;

// CVaccumCubeProcess - VaccumCube protocol processor
// Handles vaccum cube click start/cancel requests
// IDA: Main command 0x25 (eMAIN_CMD_VACCUM_CUBE)
class CVaccumCubeProcess : public TXProcess<CUser> {
public:
    // Constructor
    // IDA: ??0CVaccumCubeProcess@@QEAA@XZ @ 0x140622800
    CVaccumCubeProcess();

    // Destructor
    // IDA: ??1CVaccumCubeProcess@@UEAA@XZ @ 0x1406228C0
    virtual ~CVaccumCubeProcess();

    // Parse - Parse incoming packet
    // IDA: ?Parse@CVaccumCubeProcess@@UEAA_NAEAVXPacket@@@Z @ 0x1406228F0
    virtual bool Parse(XPacket& xPacket) override;

    // ReqVaccumClickStart - Handle vaccum click start request (subcmd 1)
    // IDA: ?ReqVaccumClickStart@CVaccumCubeProcess@@QEAA_NAEAVXPacket@@@Z @ 0x140622950
    bool ReqVaccumClickStart(XPacket& xPacket);

    // ReqVaccumClickCancel - Handle vaccum click cancel request (subcmd 2)
    // IDA: ?ReqVaccumClickCancel@CVaccumCubeProcess@@QEAA_NAEAVXPacket@@@Z @ 0x140622E40
    bool ReqVaccumClickCancel(XPacket& xPacket);
};
