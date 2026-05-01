// GMToolProcess.cpp
// CGMToolProcess GM工具包处理类实现 (对齐 IDA ControlServer.exe)

#include "GMToolProcess.h"
#include "ControlServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"

// 对齐 IDA 0x140034640: Parse - 包解析入口
bool CGMToolProcess::Parse(XPacket& xPacket) {
    // 对齐 IDA: 使用 GetSubCmd() 获取 subcmd，而非从流读取
    switch (xPacket.GetSubCmd()) {
        case 0x01: return ReqGMUserKick(xPacket);
        case 0x02: return ReqGMNotice(xPacket);
        case 0x03: return ReqGMShutDwon(xPacket);
        case 0x04: return ReqGMTimeEvent(xPacket);
        case 0x05: return ReqGMValueEvent(xPacket);
        case 0x06: return ReqGMServerOption(xPacket);
        case 0x07: return ReqGMCashShopBanner(xPacket);
        default:
            // IDA: default 返回 true (与非 GMProcess 不同)
            return true;
    }
}

// 对齐 IDA 0x140034730: ReqGMUserKick (sub 0x01)
bool CGMToolProcess::ReqGMUserKick(XPacket& xPacket) {
    ST_GM_USER_KICK_INFO stInfo{};
    xPacket >> stInfo;

    CServer* pServer = GetClientPtr();
    XControlServer::Instance()->GM_UserKick(pServer, stInfo);
    return true;
}

// 对齐 IDA 0x1400347C0: ReqGMNotice (sub 0x02)
bool CGMToolProcess::ReqGMNotice(XPacket& xPacket) {
    ST_GM_NOTICE_INFO stInfo{};
    xPacket >> stInfo;

    CServer* pServer = GetClientPtr();
    XControlServer::Instance()->GM_Notice(pServer, stInfo);
    return true;
}

// 对齐 IDA 0x140034850: ReqGMShutDwon (sub 0x03)
bool CGMToolProcess::ReqGMShutDwon(XPacket& xPacket) {
    std::uint32_t dwNo = 0;
    xPacket.XParse >> dwNo;

    CServer* pServer = GetClientPtr();
    XControlServer::Instance()->GM_Shutdown(pServer, dwNo);
    return true;
}

// 对齐 IDA 0x1400348B0: ReqGMTimeEvent (sub 0x04)
bool CGMToolProcess::ReqGMTimeEvent(XPacket& xPacket) {
    ST_GM_TIME_EVENT_INFO stInfo{};
    xPacket >> stInfo;

    CServer* pServer = GetClientPtr();
    XControlServer::Instance()->GM_TimeEvent(pServer, stInfo);
    return true;
}

// 对齐 IDA 0x140034910: ReqGMValueEvent (sub 0x05)
bool CGMToolProcess::ReqGMValueEvent(XPacket& xPacket) {
    PS_GM_VALUE_EVENT_LIST psList{};
    xPacket >> psList;

    CServer* pServer = GetClientPtr();
    XControlServer::Instance()->GM_ValueEvent(pServer, psList);
    return true;
}

// 对齐 IDA 0x1400349A0: ReqGMServerOption (sub 0x06)
bool CGMToolProcess::ReqGMServerOption(XPacket& xPacket) {
    PS_CONTENTS_INFO stInfo{};
    xPacket >> stInfo;

    CServer* pServer = GetClientPtr();
    XControlServer::Instance()->GM_ServerOption(pServer, stInfo);
    return true;
}

// 对齐 IDA 0x140034A10: ReqGMCashShopBanner (sub 0x07)
bool CGMToolProcess::ReqGMCashShopBanner(XPacket& xPacket) {
    ST_BANNER_LIST stInfo{};
    xPacket >> stInfo;

    CServer* pServer = GetClientPtr();
    XControlServer::Instance()->GM_CashShopBanner(pServer, stInfo);
    return true;
}