// WorldModeProcess.cpp
// CWorldModeProcess 世界模式包处理类实现 (对齐 IDA)

#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"
#include "Soulworker/GameServer/XControlServer/ControlServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

// 对齐 IDA 0x14004D5F0: Parse - 包解析入口
bool CWorldModeProcess::Parse(XPacket& xPacket) {
    unsigned char ucSub = 0;
    xPacket.XParse >> ucSub;

    switch (ucSub) {
        case 0x01: return ReqWorldModeCommand(xPacket);
        case 0x02: return ReqWorldModeComplete(xPacket);
        case 0x03: return ReqWorldModeUpdate(xPacket);
        case 0x04: return ReqWorldModeEnterList(xPacket);
        default:
            GreenDamTan_log(__FILE__, __FUNCTION__, "unknown sub command");
            return false;
    }
}

// 对齐 IDA 0x14004D680: ReqWorldModeCommand (sub 0x01)
bool CWorldModeProcess::ReqWorldModeCommand(XPacket& xPacket) {
    int nState = 0;
    UXMapID uxMapID{};
    std::uint32_t dwUCID = 0;
    int nModeDateID = 0;

    xPacket.XParse >> nState;
    xPacket.XParse >> uxMapID.nMapID;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> nModeDateID;

    // nState == 3: 发送模式列表
    if (nState == 3) {
        CServer* pServer = GetClientPtr();
        if (pServer) {
            ST_WORLD_MODE_INFO_VEC stModeList;
            XControlServer::Instance()->GetWorldModeManager().GetModeList(stModeList);

            XSendPacket xSendPacket(0xFB, 0x06);
            xSendPacket.XParse << dwUCID;
            xSendPacket << stModeList;
            pServer->SendEx(xSendPacket);
            return true;
        }
    }

    // 检查是否为今日模式列表
    std::vector<int> vecModeID;
    if (!XControlServer::Instance()->GetWorldModeManager().IsTodayModeList(nModeDateID, vecModeID)) {
        return false;
    }

    // nState == 1: 激活模式
    if (nState == 1) {
        int nActiveID = 0;
        int nModeID = 0;
        XControlServer::Instance()->GetWorldModeManager().GetActiveMode(nActiveID, nModeID);

        // 已有激活模式
        if (nActiveID || nModeID) {
            return false;
        }

        // 检查是否为当前激活模式
        if (!XControlServer::Instance()->GetWorldModeManager().IsActiveMode(nModeDateID, vecModeID)) {
            XControlServer::Instance()->GetWorldModeManager().SetModeStartTime(nModeDateID, vecModeID, 0);
            XControlServer::Instance()->GetWorldModeManager().SetModeFinishTime(nModeDateID, vecModeID, 0);
        }
    }
    // nState == 2: 完成模式
    else if (nState == 2) {
        int nActiveModeDateID = 0;
        int nModeID = 0;
        XControlServer::Instance()->GetWorldModeManager().GetActiveMode(nActiveModeDateID, nModeID);

        if (!nActiveModeDateID || !nModeID) {
            return false;
        }

        PS_WORLD_MODE_COMPLETE stInfo{};
        stInfo.nModeDateID = nActiveModeDateID;
        stInfo.nModeID = nModeID;
        stInfo.uxMapID = uxMapID;
        stInfo.bFinish = 1;

        XControlServer::Instance()->GetWorldModeManager().ReqModeComplete(stInfo, true);
    }

    return true;
}

// 对齐 IDA 0x14004DAF0: ReqWorldModeComplete (sub 0x02)
bool CWorldModeProcess::ReqWorldModeComplete(XPacket& xPacket) {
    PS_WORLD_MODE_COMPLETE stComplete{};
    std::uint32_t dwMonsterID = 0;

    xPacket >> stComplete;
    xPacket.XParse >> dwMonsterID;

    CServer* pServer = GetClientPtr();
    if (!pServer) return false;

    if (XControlServer::Instance()->GetWorldModeManager().ReqModeComplete(stComplete, false)) {
        XSendPacket xSendPacket(0xFB, 0x07);
        xSendPacket << stComplete;
        xSendPacket.XParse << dwMonsterID;
        pServer->SendEx(xSendPacket);
    }
    return true;
}

// 对齐 IDA 0x14004DC20: ReqWorldModeUpdate (sub 0x03)
bool CWorldModeProcess::ReqWorldModeUpdate(XPacket& xPacket) {
    PS_WORLD_MODE_UPDATE stUpdate{};
    xPacket >> stUpdate;

    XControlServer::Instance()->GetWorldModeManager().UpdateMonsterCount(stUpdate);
    return true;
}

// 对齐 IDA 0x14004DC90: ReqWorldModeEnterList (sub 0x04)
bool CWorldModeProcess::ReqWorldModeEnterList(XPacket& xPacket) {
    std::uint32_t dwUCID = 0;
    ST_ENTER_WORLD_MODE_INFO stEnterInfo{};

    xPacket.XParse >> dwUCID;
    xPacket >> stEnterInfo;

    auto pUser = XControlServer::Instance()->GetUser(dwUCID);
    if (pUser) {
        pUser->LoadEnterWorldModeInfo(stEnterInfo);
    }
    return true;
}