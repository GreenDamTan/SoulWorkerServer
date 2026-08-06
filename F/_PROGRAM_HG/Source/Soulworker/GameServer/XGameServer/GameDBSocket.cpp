// GameDBSocket.cpp
// XGameDBSocket implementation
// IDA decompilation from GameServer.exe

#include "GameDBSocket.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/Process/LoginProcess.h"
#include "Soulworker/GameServer/XGameServer/Process/CharacterProcess.h"
#include "Soulworker/GameServer/XGameServer/Process/ItemProcess.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerChat.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_TimeCompat.h"
#include <cwchar>

// FindUser - 根据会话ID查找用户
// IDA 0x140308940: TXSingleton<XGameServer>::Instance() -> TXServer<CUser>::FindUser.
CUser* XGameDBSocket::FindUser(int xSessionID) {
    XGameServer* server = TXSingleton<XGameServer>::Instance();
    return server ? server->TXServer<CUser>::FindUser(xSessionID) : nullptr;
}

// DBParse - 数据库回包主命令分发
// IDA 0x140308A00: only the currently compiled process parsers are wired here.
bool XGameDBSocket::DBParse(CUser* pUser, XPacket& xPacket) {
    const std::uint8_t byMainCmd = xPacket.GetMainCmd();
    const std::uint8_t bySubCmd = xPacket.GetSubCmd();

    if (pUser && !pUser->IsState(XClient::eStateConnect)) {
        return false;
    }

    ++m_nRecvCount;
    if (m_dwRecvTick < GreenDamTan::GetTickCount64Compat()) {
        LogHelper::LogInfo("game.system",
                           "[JPN] XGameDBSocket::DBParse %p / %d",
                           static_cast<void*>(this),
                           m_nRecvCount);
        m_dwRecvTick = GreenDamTan::GetTickCount64Compat() + 60000;
        m_nRecvCount = 0;
    }

    switch (byMainCmd) {
    case 1:
        return SystemDBParse(pUser, bySubCmd, xPacket);
    case 2: {
        CLoginProcess* process =
            pUser ? pUser->GetProcessPtr<CLoginProcess>(2) : nullptr;
        return process ? process->DBParse(xPacket) : true;
    }
    case 3: {
        CCharacterProcess* process =
            pUser ? pUser->GetProcessPtr<CCharacterProcess>(3) : nullptr;
        return process ? process->DBParse(xPacket) : true;
    }
    case 0x21: {
        CItemProcess* process =
            pUser ? pUser->GetProcessPtr<CItemProcess>(8) : nullptr;
        return process ? process->DBItemParse(xPacket) : true;
    }
    default:
        (void)bySubCmd;
        return true;
    }
}

bool XGameDBSocket::SystemDBParse(CUser* pUser,
                                  std::uint8_t bySubCmd,
                                  XPacket& xPacket) {
    switch (bySubCmd) {
    case 1:
        ResConnectHanNetCafeList(xPacket);
        break;
    case 3:
        ResWorldEventInfoSync(xPacket);
        break;
    case 4:
        ResGetDBTime(pUser, xPacket);
        break;
    }
    return true;
}

bool XGameDBSocket::ResConnectHanNetCafeList(XPacket& xPacket) {
    PS_DB_HAN_NET_CAFE_LIST psNetCafeList;
    xPacket >> psNetCafeList;
    return true;
}

bool XGameDBSocket::ResWorldEventInfoSync(XPacket& xPacket) {
    PS_WORLD_EVENT_INFO_RES psDBRes;
    xPacket >> psDBRes;

    CLogicThreadManager::Instance().DoCommonJob(
        [psDBRes]() {
            XGameServer* pServer = TXSingleton<XGameServer>::Instance();
            if (pServer) {
                pServer->GetWorldEventMgr().UpdateTotalCount(
                    psDBRes.nEventID,
                    psDBRes.nTotalCount);
            }
        });
    return true;
}

bool XGameDBSocket::ResGetDBTime(CUser* pUser, XPacket& xPacket) {
    PS_CHAT_NOTICE psInfo;
    xPacket >> psInfo;

    if (!pUser || !pUser->GetArea()) {
        return false;
    }

    pUser->IncrementJobCount();
    const UXMapID mapID = pUser->GetMapInsID();
    const bool bSent = CLogicThreadManager::Instance().DoJob(
        mapID.nMapID,
        [pUser, psInfo]() {
            PS_CHAT_NOTICE stChat;
            stChat.byType = 0;
            std::swprintf(stChat.strMsg,
                          sizeof(stChat.strMsg) / sizeof(stChat.strMsg[0]),
                          L"[DB time : %s]",
                          psInfo.strMsg);
            XSendPacket xSendPacket(7, 4);
            xSendPacket << stChat;
            CGocNetwork::Send(static_cast<XActor*>(pUser), xSendPacket);
        });
    (void)bSent;

    const UXMapID cleanupMapID = pUser->GetMapInsID();
    return CLogicThreadManager::Instance().DoJob(
        cleanupMapID.nMapID,
        [pUser]() {
            pUser->DecrementJobCount();
        });
}

// OnDisConnect - 断开连接处理
void XGameDBSocket::OnDisConnect() {
    LogHelper::LogError("game.system", "[DB_SOCKET] OnDisconnect (type=%d)", m_byType);
    
    // 调用基类
    TXDBSocketT<CUser>::OnDisConnect();
}

// OnNotConnect - 连接失败处理
void XGameDBSocket::OnNotConnect() {
    LogHelper::LogError("game.system", "[DB_SOCKET] OnNotConnect (type=%d)", m_byType);
    
    // 调用基类
    TXDBSocketT<CUser>::OnNotConnect();
}
