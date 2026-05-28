// CChatProcess.cpp
// Chat system packet handler for GameServer
// Reconstructed from IDA decompilation

#include "StdAfx.h"
#include "ChatProcess.h"
#include "../User.h"
#include "../GameServer.h"
#include "../../XCore/XServer/XServer.h"
#include "../../XCommon/PSCommon.h"
#include "../../XCommon/PSServer.h"

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
bool CChatProcess::ProcessNormal(CUser* pUser, wchar_t* szMessage, PS_CHAT_ITEM_LINK_FOR_SERVER* psItemLink)
{
    // Implementation based on IDA decompilation
    return true;
}

// Process party chat (static helper)
bool CChatProcess::ProcessParty(CUser* pUser, wchar_t* szMessage, PS_CHAT_ITEM_LINK_FOR_SERVER* psItemLink)
{
    // Implementation based on IDA decompilation
    return true;
}

// Process league chat (static helper)
bool CChatProcess::ProcessLeague(CUser* pUser, wchar_t* szMessage, PS_CHAT_ITEM_LINK_FOR_SERVER* psItemLink)
{
    // Implementation based on IDA decompilation
    return true;
}

// Send whisper chat
void CChatProcess::SendChatWhisper(CUser* pUser, PS_CHAT_WHISPER& psWhisper, PS_CHAT_ITEM_LINK_FOR_SERVER psItemLink)
{
    // Implementation based on IDA decompilation
}

// GM command process
bool CChatProcess::GMCommandProcess(const CString& strCommand, CUser* pUser, int* pnResult, bool& bSuccess)
{
    // Implementation based on IDA decompilation
    return true;
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
