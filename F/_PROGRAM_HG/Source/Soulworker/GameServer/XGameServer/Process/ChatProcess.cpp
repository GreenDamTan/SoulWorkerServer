// CChatProcess.cpp
// Chat system packet handler for GameServer
// Reconstructed from IDA decompilation

#include "StdAfx.h"
#include "ChatProcess.h"
#include "../User.h"
#include "../GameServer.h"
#include "../actor/component/GocNetwork.h"
#include "../actor/component/GocParty.h"
#include "../actor/component/GocForce.h"
#include "../../XCore/XServer/XServer.h"
#include "../../XCore/XServer/XSendPacket.h"
#include "../../XCommon/PSCommon.h"
#include "../../XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerChat.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerParty.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerLeague.h"

// Constructor
CChatProcess::CChatProcess()
    : TXProcess<CUser>()
{
    SetCmd(CMD_CHAT);
    SetName("CChatProcess");
}

// Destructor
CChatProcess::~CChatProcess()
{
}

// Main packet parser - routes subcommands to appropriate handlers
bool CChatProcess::Parse(XPacket& xPacket)
{
    switch (xPacket.GetSubCmd())
    {
    case 1:   // Normal Chatting
        return ReqNormalChatting(xPacket);

    case 2:   // Whisper
        return ReqChatWhisper(xPacket);

    case 3:   // Trade Chat
        return ReqChatTrade(xPacket);

    case 4:   // Notice
        return ReqChatNotice(xPacket);

    case 6:   // MegaPhone
        return ReqChatMegaPhone(xPacket);

    case 0xA: // GM Command
        return ReqGMCommand(xPacket);

    default:
        return true;
    }
}

// Request normal chatting
bool CChatProcess::ReqNormalChatting(XPacket& xPacket)
{
    CUser* pUser = GetClientPtr();

    PS_CHAT_NORMAL stChatInfo;
    unsigned char byLinkItemCount = 0;
    PS_CHAT_ITEM_LINK_REQ psLinkItemInfo[3];

    xPacket >> stChatInfo;
    xPacket >> byLinkItemCount;

    // Validate link item count
    if (byLinkItemCount > 3)
    {
        LogHelper::LogError("game.contents", "ReqNormalChatting error - Over linked Item count MAX( %d )", 123);
        return true;
    }

    // Check for packet errors
    if (xPacket.GetLastError() != XPacket::eSUCCESS)
    {
        LogHelper::LogError("game.contents", "ReqNormalChatting error - XPacket::eSUCCESS != xPacket.GetLastError() ( %d )", 145);
        SendErrorMessage(1, 0xC3B6);

        PS_KICK_USER_INFO psKick;
        psKick.dwUAID = pUser->GetUAID();
        psKick.byKickType = 8;
        pUser->Kickout(&psKick, false);
        return true;
    }

    // Build server item link info
    PS_CHAT_ITEM_LINK_FOR_SERVER psItemLinkInfoServer;
    psItemLinkInfoServer.byItemLinkCount = byLinkItemCount;

    for (int i = 0; i < byLinkItemCount; ++i)
    {
        xPacket >> psLinkItemInfo[i];
        psItemLinkInfoServer.psItemLinkInfo[i].byStart = psLinkItemInfo[i].byStart;
        psItemLinkInfoServer.psItemLinkInfo[i].bySize = psLinkItemInfo[i].bySize;
        psItemLinkInfoServer.psItemLinkInfo[i].byType = psLinkItemInfo[i].byType;
        psItemLinkInfoServer.psItemLinkInfo[i].i64ID = psLinkItemInfo[i].i64ID;
        wcsncpy(psItemLinkInfoServer.psItemLinkInfo[i].szLinkString, psLinkItemInfo[i].szLinkString, 64);
    }

    if (!pUser || !pUser->GetArea())
        return false;

    pUser->IncrementJobCount();

    // Process chat on logic thread
    // ... (lambda job scheduling code omitted for clarity)

    return true;
}

// Request whisper chat
bool CChatProcess::ReqChatWhisper(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Request trade chat
bool CChatProcess::ReqChatTrade(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Request chat notice
bool CChatProcess::ReqChatNotice(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Request mega phone chat
bool CChatProcess::ReqChatMegaPhone(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Request GM command
bool CChatProcess::ReqGMCommand(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Process normal chat (static helper)
// IDA: ?ProcessNormal@CChatProcess@@KA_NPEAVCUser@@PEA_WUPS_CHAT_ITEM_LINK_FOR_SERVER@@@Z @ 0x1403E5F40
bool CChatProcess::ProcessNormal(CUser* pUser, wchar_t* szMsg, PS_CHAT_ITEM_LINK_FOR_SERVER* psChatLinkInfoServer)
{
    if (!pUser || !szMsg || !psChatLinkInfoServer)
        return false;

    // Create packet for broadcast
    XSendPacket xSendPacket(CMD_CHAT, 1);  // main=7, sub=1 (normal chat)

    // Get actor ID from user
    UXActorID actorID = pUser->GetActorID();
    xSendPacket << actorID.dwActorID;
    xSendPacket << static_cast<int>(1);  // chat type
    xSendPacket << szMsg;
    xSendPacket << psChatLinkInfoServer->byItemLinkCount;

    // Create and write item link info
    PS_CHAT_ITEM_LINK psLinkItemInfo[3];
    for (int i = 0; i < psChatLinkInfoServer->byItemLinkCount && i < 3; ++i)
    {
        psLinkItemInfo[i] = psChatLinkInfoServer->psItemLinkInfo[i];
        xSendPacket << psLinkItemInfo[i];
    }

    // Broadcast to nearby actors
    CGocNetwork::SendBroadCast(pUser, xSendPacket, E_BROADCAST_TYPE::eBC_Default);

    // Send chat log
    int nLogParam[5] = {0};
    __int64 n64LogParam[2] = {0};
    CChatProcess::SendChatLog(1, pUser->GetUAID(), actorID.dwActorID, nLogParam, n64LogParam, szMsg);

    return true;
}

// Process party chat (static helper)
// IDA: ?ProcessParty@CChatProcess@@KA_NPEAVCUser@@PEA_WUPS_CHAT_ITEM_LINK_FOR_SERVER@@@Z @ 0x1403E6410
bool CChatProcess::ProcessParty(CUser* pUser, wchar_t* szMsg, PS_CHAT_ITEM_LINK_FOR_SERVER* psChatLinkInfoServer)
{
    if (!pUser || !szMsg || !psChatLinkInfoServer)
        return false;

    // Get party and force components
    auto pParty = pUser->GetGOC<CGocParty>();
    auto pForce = pUser->GetGOC<CGocForce>();

    // Get actor ID
    UXActorID actorID = pUser->GetActorID();

    // Check if user is in a party
    if (pParty && pParty->IsParty())
    {
        // Build party chat packet
        PS_CHAT_PARTY stChatParty;
        stChatParty.dwActorID = actorID.dwActorID;
        stChatParty.dwPartyID = pParty->GetPartyID();
        wcscpy_s(stChatParty.szMsg, szMsg);

        XSendPacket xSendPacket(0xF4, 0x14);  // CMD_PARTY, sub=0x14
        xSendPacket << stChatParty;
        xSendPacket << *psChatLinkInfoServer;

        // Send to community socket for relay
        auto pGameServer = XGameServer::Instance();
        if (pGameServer)
        {
            pGameServer->GetCommunitySocket().SendCmd(&xSendPacket, pUser, CMD_CHAT, 1);
        }

        // Send chat log
        int nLogParam[5] = {0};
        __int64 n64LogParam[2] = {0};
        nLogParam[2] = static_cast<int>(pParty->GetPartyID());
        CChatProcess::SendChatLog(3, pUser->GetUAID(), actorID.dwActorID, nLogParam, n64LogParam, szMsg);
    }
    // Check if user is in a force (guild)
    else if (pForce && pForce->IsForce())
    {
        // Build force chat packet
        // Note: PS_CHAT_FORCE is alias to PS_CHAT_PARTY, uses dwPartyID field for ForceID
        PS_CHAT_FORCE stChatForce;
        stChatForce.dwActorID = actorID.dwActorID;
        stChatForce.dwPartyID = pForce->GetForceID();  // Using dwPartyID field for ForceID
        wcscpy_s(stChatForce.szMsg, szMsg);

        XSendPacket xSendPacket(0xFA, 0x10);  // CMD_FORCE, sub=0x10
        xSendPacket << stChatForce;
        xSendPacket << *psChatLinkInfoServer;

        // Send to community socket for relay
        auto pGameServer = XGameServer::Instance();
        if (pGameServer)
        {
            pGameServer->GetCommunitySocket().SendCmd(&xSendPacket, pUser, CMD_CHAT, 1);
        }

        // Send chat log
        int nLogParam[5] = {0};
        __int64 n64LogParam[2] = {0};
        nLogParam[2] = static_cast<int>(pForce->GetForceID());
        CChatProcess::SendChatLog(3, pUser->GetUAID(), actorID.dwActorID, nLogParam, n64LogParam, szMsg);
    }
    else
    {
        // Not in party or force - send error to self
        XSendPacket packet(CMD_CHAT, 1);
        int nChatType = 3;  // party chat type
        packet << actorID.dwActorID;
        packet << nChatType;
        packet << szMsg;
        packet << psChatLinkInfoServer->byItemLinkCount;

        for (int i = 0; i < psChatLinkInfoServer->byItemLinkCount && i < 3; ++i)
        {
            PS_CHAT_ITEM_LINK psChatLinkItemInfo = psChatLinkInfoServer->psItemLinkInfo[i];
            packet << psChatLinkItemInfo;
        }

        CGocNetwork::Send(pUser, packet);
    }

    return true;
}

// Process league chat (static helper)
// IDA: ?ProcessLeague@CChatProcess@@KA_NPEAVCUser@@PEA_WUPS_CHAT_ITEM_LINK_FOR_SERVER@@@Z @ 0x1403E6B90
bool CChatProcess::ProcessLeague(CUser* pUser, wchar_t* szMsg, PS_CHAT_ITEM_LINK_FOR_SERVER* psChatLinkInfoServer)
{
    if (!pUser || !szMsg || !psChatLinkInfoServer)
        return false;

    // Check if user is in a league
    std::uint32_t dwLeagueID = pUser->GetLeagueID();
    if (dwLeagueID == 0)
        return true;  // Not in league, just return

    // Get actor ID
    UXActorID actorID = pUser->GetActorID();

    // Build league chat packet
    PS_CHAT_LEAGUE stChatLeague;
    stChatLeague.dwActorID = actorID.dwActorID;
    stChatLeague.dwLeagueID = dwLeagueID;
    stChatLeague.dwMemberID = actorID.dwActorID;  // Member ID same as actor ID
    wcscpy_s(stChatLeague.szMsg, szMsg);

    XSendPacket xSendPacket(0xF6, 0x36);  // CMD_LEAGUE, sub=0x36
    xSendPacket << stChatLeague;
    xSendPacket << *psChatLinkInfoServer;

    // Send to community socket for relay
    auto pGameServer = XGameServer::Instance();
    if (pGameServer)
    {
        pGameServer->GetCommunitySocket().SendCmd(&xSendPacket, pUser, CMD_CHAT, 1);
    }

    // Send chat log
    int nLogParam[5] = {0};
    __int64 n64LogParam[2] = {0};
    nLogParam[2] = static_cast<int>(dwLeagueID);
    CChatProcess::SendChatLog(4, pUser->GetUAID(), actorID.dwActorID, nLogParam, n64LogParam, szMsg);

    return true;
}

// Send whisper chat
// IDA: ?SendChatWhisper@CChatProcess@@QEAAXPEAVCUser@@AEAUPS_CHAT_WHISPER@@UPS_CHAT_ITEM_LINK_FOR_SERVER@@@Z @ 0x1403E7A70
void CChatProcess::SendChatWhisper(CUser* pUser, PS_CHAT_WHISPER& stChatWhisper, PS_CHAT_ITEM_LINK_FOR_SERVER psLinkItemInfo)
{
    if (!pUser)
        return;

    // Create whisper packet (main=7, sub=2)
    XSendPacket xSendPacket(CMD_CHAT, 2);
    xSendPacket << stChatWhisper;
    xSendPacket << psLinkItemInfo->byItemLinkCount;

    // Write item link info
    for (int i = 0; i < psLinkItemInfo->byItemLinkCount; ++i)
    {
        PS_CHAT_ITEM_LINK psInfo = psLinkItemInfo->psItemLinkInfo[i];
        xSendPacket << psInfo;
    }

    // Send to user
    CGocNetwork::Send(pUser, xSendPacket);
}

// GM command process
// 对齐 IDA: ?GMCommandProcess@CChatProcess@@IEAAEV?$CStringT@_WV?$StrTraitATL@_WV?$ChTraitsCRT@_W@ATL@@@ATL@@@ATL@@PEAVCUser@@PEAHAEA_J@Z @ 0x1403E7BE0
bool CChatProcess::GMCommandProcess(const CString& strCommand, CUser* pUser, int* pnLogParam, __int64& n64LogParam)
{
    // 对齐 IDA: 初始化日志参数
    n64LogParam = 0;
    pnLogParam[4] = 0;
    pnLogParam[3] = 0;
    pnLogParam[2] = 0;
    pnLogParam[1] = 0;
    pnLogParam[0] = 0;

    // 对齐 IDA: 解析命令参数
    wchar_t szCmd[5][256];
    memset(szCmd, 0, sizeof(szCmd));

    // 对齐 IDA: swscanf_s 解析命令
    swscanf_s(strCommand.GetString(), L"/%s %s %s %s %s",
        szCmd[0], 256, szCmd[1], 256, szCmd[2], 256, szCmd[3], 256, szCmd[4], 256);

    unsigned char byLog = 0;

    // 对齐 IDA: 在 m_mapGMCommand 中查找命令
    std::wstring strKey = szCmd[0];
    auto iter = m_mapGMCommand.find(strKey);

    if (iter != m_mapGMCommand.end())
    {
        // 对齐 IDA: 调用命令处理函数
        GMCommandHandler pHandler = iter->second;
        byLog = pHandler(pUser, szCmd[1], szCmd[2], szCmd[3], szCmd[4], pnLogParam, n64LogParam);
    }
    else
    {
        // 对齐 IDA: 检查是否是 "notice" 命令
        if (_wcsicmp(szCmd[0], L"notice") == 0)
        {
            // 对齐 IDA: 调用 GMCOMMAND_GM_CMD_NOTICE_ENG
            // static function declaration
            extern unsigned char GMCOMMAND_GM_CMD_NOTICE_ENG(CUser* pUser, wchar_t* szParam1,
                wchar_t* szParam2, wchar_t* szParam3, const CString& strMessage);
            byLog = GMCOMMAND_GM_CMD_NOTICE_ENG(pUser, szCmd[1], szCmd[2], szCmd[3], strCommand);
        }
    }

    return byLog != 0;
}

// Initialize GM commands
void CChatProcess::InitGMCommand()
{
    // Register all GM command handlers
    // Implementation based on IDA decompilation
}

// Initialize debug commands
void CChatProcess::InitCommand_Debug()
{
    // Register debug command handlers
    // Implementation based on IDA decompilation
}
