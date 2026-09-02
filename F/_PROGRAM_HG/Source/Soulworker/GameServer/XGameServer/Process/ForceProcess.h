// ForceProcess.h
// XForceProcess - Force/Guild 客户端请求处理器 (main cmd 0x2E)
// 对齐 IDA GameServer.exe；原始归属 process/ForceProcess.cpp
// (PDB cvdump MD5 0CE935F8D4EFBEDB2196DCD00B793D89)

#pragma once

#include "../../XCore/XIOCPServer/TXProcess.h"
#include <cstdint>

class CUser;
class XPacket;
struct PS_SERVER_FORCE_MATCHING_ENTER;

// ============================================================================
// XForceProcess - Force 请求处理类 (main=0x2E)
//
// 客户端 -> GameServer 的 Force（公会）请求处理：
// - Parse 按 subcmd 分发到 ReqForce* 处理器
// - CheckForceMatchingEnterUser 用于匹配进入前的条件校验
// ============================================================================
class XForceProcess : public TXProcess<CUser> {
public:
    // IDA: ??0XForceProcess@@QEAA@XZ @ 0x140430B00
    XForceProcess();

    // IDA: ??1XForceProcess@@UEAA@XZ @ 0x140430BC0
    virtual ~XForceProcess();

    // === 主分发 ===
    // IDA: ?Parse@XForceProcess@@UEAA_NAEAVXPacket@@@Z @ 0x140430BF0
    virtual bool Parse(XPacket& xPacket) override;

    // === 请求处理器 ===
    // IDA: ?ReqForceInvite@XForceProcess@@QEAA_NAEAVXPacket@@@Z @ 0x140430D40
    bool ReqForceInvite(XPacket& xPacket);
    // IDA: ?ReqForceAccept@XForceProcess@@QEAA_NAEAVXPacket@@@Z @ 0x1404319F0
    bool ReqForceAccept(XPacket& xPacket);
    // IDA: ?ReqForceChangeMaster@XForceProcess@@QEAA_NAEAVXPacket@@@Z @ 0x140432790
    bool ReqForceChangeMaster(XPacket& xPacket);
    // IDA: ?ReqForceKickOut@XForceProcess@@QEAA_NAEAVXPacket@@@Z @ 0x140432A20
    bool ReqForceKickOut(XPacket& xPacket);
    // IDA: ?ReqForceLeave@XForceProcess@@QEAA_NXZ @ 0x140432D40
    bool ReqForceLeave();
    // IDA: ?ReqForceCancel@XForceProcess@@QEAA_NAEAVXPacket@@@Z @ 0x140432370
    bool ReqForceCancel(XPacket& xPacket);
    // IDA: ?ReqForceMatchingEnter@XForceProcess@@QEAA_NAEAVXPacket@@@Z @ 0x140432FD0
    bool ReqForceMatchingEnter(XPacket& xPacket);
    // IDA: ?ReqForceMatchingExit@XForceProcess@@QEAA_NXZ @ 0x140433B90
    bool ReqForceMatchingExit();
    // IDA: ?ReqForceMatchingCheck@XForceProcess@@QEAA_NAEAVXPacket@@@Z @ 0x140433F20
    bool ReqForceMatchingCheck(XPacket& xPacket);

    // === 匹配进入校验 ===
    // IDA: ?CheckForceMatchingEnter@XForceProcess@@QEAA_NPEAVCUser@@GAEAHAEAUPS_SERVER_FORCE_MATCHING_ENTER@@1@Z @ 0x1404342A0
    bool CheckForceMatchingEnter(CUser* pUser, std::uint16_t wMazeID, int& nError,
                                  PS_SERVER_FORCE_MATCHING_ENTER& stEnter, int* pOut);

    // IDA: ?CheckForceMatchingEnterUser@XForceProcess@@QEAA_NPEAVCUser@@GAEAH1@Z @ 0x140435020
    bool CheckForceMatchingEnterUser(CUser* pUser, std::uint16_t wMazeID,
                                     int& nError, int* nNeedItemID);
};
