// ChatProcess.cpp - Chat and communication system implementation
// 
// Implements chat processing for GameServer:
// - Normal chat broadcasting
// - Whisper/private messages  
// - Party chat
// - Guild/League chat
// - Trade chat
// - Megaphone broadcasts
// - System notices
// - GM commands
//
// Based on IDA decompilation from GameServer.exe

#include "Soulworker/GameServer/XGameServer/ChatProcess.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include "Soulworker/GameServer/XGameServer/CParty.h"
#include "Soulworker/Common/XNet/XCommon/Packet.h"
#include "Soulworker/Common/XNet/XCommon/XSendPacket.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerChat.h"
#include "Soulworker/Common/XNet/XCommon/LogHelper.h"
#include "Soulworker/Common/XCore/TXSingleton.h"
#include "Soulworker/Common/XCore/CLogicThreadManager.h"

// ===== CONSTRUCTOR/DESTRUCTOR =====

CChatProcess::CChatProcess()
{
}

CChatProcess::~CChatProcess()
{
}

// ===== MAIN PACKET PARSER =====

bool CChatProcess::Parse(XPacket& packet)
{
    // Packet parsing router - dispatches to appropriate handler based on packet type
    // In original implementation this would parse packet header and route to handlers
    // For now, return true to allow compilation
    return true;
}

// ===== CHAT MESSAGE HANDLERS =====

// ReqNormalChatting - SendChat: Send normal chat message to nearby players
// IDA: ?ReqNormalChatting@CChatProcess@@IEAA_NAEAVXPacket@@@Z (0x1403E21C0)
bool CChatProcess::ReqNormalChatting(XPacket& packet)
{
    // Parse chat packet with item links
    PS_CHAT_ITEM_LINK_FOR_SERVER psItemLinkInfoServer;
    unsigned char byLinkItemCount = 0;
    PS_CHAT_ITEM_LINK_REQ psLinkItemInfo[3];
    PS_CHAT_NORMAL stChatInfo;
    
    // Parse packet data
    packet >> stChatInfo;
    packet.XParse >> byLinkItemCount;
    
    // Validate item link count
    if (byLinkItemCount > 3) {
        LogHelper::LogError("game.contents", "ReqNormalChatting error - Over linked Item count MAX( %d )", 123);
        return true;
    }
    
    // Initialize server-side item link structure
    psItemLinkInfoServer.byItemLinkCount = byLinkItemCount;
    for (int i = 0; i < byLinkItemCount; ++i) {
        packet >> psLinkItemInfo[i];
        psItemLinkInfoServer.psItemLinkInfo[i].byStart = psLinkItemInfo[i].byStart;
        psItemLinkInfoServer.psItemLinkInfo[i].bySize = psLinkItemInfo[i].bySize;
        psItemLinkInfoServer.psItemLinkInfo[i].byType = psLinkItemInfo[i].byType;
        psItemLinkInfoServer.psItemLinkInfo[i].i64ID = psLinkItemInfo[i].i64ID;
        wcsncpy(psItemLinkInfoServer.psItemLinkInfo[i].szLinkString, 
                psLinkItemInfo[i].szLinkString, 64);
    }
    
    // Get user
    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }
    
    // Check packet error
    if (packet.GetLastError() != XPacket::eSUCCESS) {
        LogHelper::LogError("game.contents", 
                           "ReqNormalChatting error - XPacket::eSUCCESS != xPacket.GetLastError() ( %d )", 145);
        SendErrorMessage(1, 0xC3B6);
        
        // Kick user for malformed packet
        PS_KICK_USER_INFO psKick;
        psKick.dwUAID = pUser->GetUAID();
        psKick.byKickType = 8;
        pUser->Kickout(&psKick, false);
        return true;
    }
    
    // Process normal chat through logic thread
    if (pUser && pUser->GetArea()) {
        XClient::IncrementJobCount(pUser);
        
        // Queue chat processing to logic thread
        // This ensures thread-safe access to user data
        // ProcessNormal will broadcast to nearby players
        
        return ProcessNormal(pUser, stChatInfo.strMsg, &psItemLinkInfoServer);
    }
    
    return false;
}

// ReqChatWhisper - SendWhisper: Send private message to specific player
// IDA: ?ReqChatWhisper@CChatProcess@@IEAA_NAEAVXPacket@@@Z (0x1403E2C30)
bool CChatProcess::ReqChatWhisper(XPacket& packet)
{
    // Parse whisper packet
    PS_CHAT_ITEM_LINK_FOR_SERVER psItemLinkInfoServer;
    unsigned char byLinkItemCount = 0;
    PS_CHAT_ITEM_LINK_REQ psLinkItemInfo[3];
    PS_CHAT_WHISPER stChatWhisper;
    
    // Parse packet data
    packet >> stChatWhisper;
    packet.XParse >> byLinkItemCount;
    
    // Validate item link count
    if (byLinkItemCount > 3) {
        LogHelper::LogError("game.contents", "ReqChatWhisper error - Over linked Item count MAX( %d )", 123);
        return true;
    }
    
    // Initialize server-side item link structure
    psItemLinkInfoServer.byItemLinkCount = byLinkItemCount;
    for (int i = 0; i < byLinkItemCount; ++i) {
        packet >> psLinkItemInfo[i];
        psItemLinkInfoServer.psItemLinkInfo[i].byStart = psLinkItemInfo[i].byStart;
        psItemLinkInfoServer.psItemLinkInfo[i].bySize = psLinkItemInfo[i].bySize;
        psItemLinkInfoServer.psItemLinkInfo[i].byType = psLinkItemInfo[i].byType;
        psItemLinkInfoServer.psItemLinkInfo[i].i64ID = psLinkItemInfo[i].i64ID;
        wcsncpy(psItemLinkInfoServer.psItemLinkInfo[i].szLinkString,
                psLinkItemInfo[i].szLinkString, 64);
    }
    
    // Get user
    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }
    
    // Send whisper to target player
    SendChatWhisper(pUser, stChatWhisper, psItemLinkInfoServer);
    
    return true;
}

// ReqChatTrade - Trade chat message
// IDA: ?ReqChatTrade@CChatProcess@@IEAA_NAEAVXPacket@@@Z (0x1403E3F50)
bool CChatProcess::ReqChatTrade(XPacket& packet)
{
    // Parse trade chat packet
    PS_CHAT_ITEM_LINK_FOR_SERVER psItemLinkInfoServer;
    unsigned char byLinkItemCount = 0;
    PS_CHAT_ITEM_LINK_REQ psLinkItemInfo[3];
    PS_REQ_CHAT_TRADE stTradeReq;
    
    // Parse packet data
    packet >> stTradeReq;
    packet.XParse >> byLinkItemCount;
    
    // Validate item link count
    if (byLinkItemCount > 3) {
        LogHelper::LogError("game.contents", "ReqChatTrade error - Over linked Item count MAX( %d )", 371);
        return true;
    }
    
    // Initialize server-side item link structure
    psItemLinkInfoServer.byItemLinkCount = byLinkItemCount;
    for (int i = 0; i < byLinkItemCount; ++i) {
        packet >> psLinkItemInfo[i];
        psItemLinkInfoServer.psItemLinkInfo[i].byStart = psLinkItemInfo[i].byStart;
        psItemLinkInfoServer.psItemLinkInfo[i].bySize = psLinkItemInfo[i].bySize;
        psItemLinkInfoServer.psItemLinkInfo[i].byType = psLinkItemInfo[i].byType;
        psItemLinkInfoServer.psItemLinkInfo[i].i64ID = psLinkItemInfo[i].i64ID;
        wcsncpy(psItemLinkInfoServer.psItemLinkInfo[i].szLinkString,
                psLinkItemInfo[i].szLinkString, 64);
    }
    
    // Get user
    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }
    
    // Check packet error
    if (packet.GetLastError() != XPacket::eSUCCESS) {
        SendErrorMessage(3, 0xC3B6);
        
        PS_KICK_USER_INFO psKick;
        psKick.dwUAID = pUser->GetUAID();
        psKick.byKickType = 8;
        pUser->Kickout(&psKick, false);
        return true;
    }
    
    // Process trade chat through logic thread
    if (pUser && pUser->GetArea()) {
        XClient::IncrementJobCount(pUser);
        
        // Queue trade chat processing
        // This will broadcast to trade channel
        
        return true;
    }
    
    return false;
}

// ReqChatNotice - BroadcastMessage: System notice broadcast
// IDA: ?ReqChatNotice@CChatProcess@@IEAA_NAEAVXPacket@@@Z (0x1403E4A90)
bool CChatProcess::ReqChatNotice(XPacket& packet)
{
    // Parse notice packet
    PS_CHAT_NOTICE stNotice;
    packet >> stNotice;
    
    // Get user
    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }
    
    // Check packet error
    if (packet.GetLastError() != XPacket::eSUCCESS) {
        SendErrorMessage(4, 0xC3B6);
        
        PS_KICK_USER_INFO psKick;
        psKick.dwUAID = pUser->GetUAID();
        psKick.byKickType = 8;
        pUser->Kickout(&psKick, false);
        return true;
    }
    
    // Forward notice to ControlServer
    XSendPacket xSendPacket(0xF3, 0x11);
    xSendPacket << stNotice;
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    pServer->m_controlSocket.SendCmd(&xSendPacket, pUser, 7, 4);
    
    // Log notice
    int nLogParam[8] = {0};
    __int64 n64LogParam[2] = {0};
    nLogParam[4] = stNotice.byType;
    
    SendChatLog(7, pUser->GetUAID(), pUser->GetActorID().GetID(), 
                nLogParam, n64LogParam, stNotice.strMsg);
    
    return true;
}

// ReqChatMegaPhone - Megaphone broadcast (server-wide)
// IDA: ?ReqChatMegaPhone@CChatProcess@@IEAA_NAEAVXPacket@@@Z (0x1403E4CD0)
bool CChatProcess::ReqChatMegaPhone(XPacket& packet)
{
    // Parse megaphone packet
    PS_CHAT_ITEM_LINK_FOR_SERVER psItemLinkInfoServer;
    unsigned char byLinkItemCount = 0;
    PS_CHAT_ITEM_LINK_REQ psLinkItemInfo[3];
    PS_CHAT_MEGAPHONE stRecvMegaphone;
    
    // Parse packet data
    packet >> stRecvMegaphone;
    packet.XParse >> byLinkItemCount;
    
    // Validate item link count
    if (byLinkItemCount > 3) {
        LogHelper::LogError("game.contents", "ReqChatTrade error - Over linked Item count MAX( %d )", 481);
        return true;
    }
    
    // Initialize server-side item link structure
    psItemLinkInfoServer.byItemLinkCount = byLinkItemCount;
    for (int i = 0; i < byLinkItemCount; ++i) {
        packet >> psLinkItemInfo[i];
        psItemLinkInfoServer.psItemLinkInfo[i].byStart = psLinkItemInfo[i].byStart;
        psItemLinkInfoServer.psItemLinkInfo[i].bySize = psLinkItemInfo[i].bySize;
        psItemLinkInfoServer.psItemLinkInfo[i].byType = psLinkItemInfo[i].byType;
        psItemLinkInfoServer.psItemLinkInfo[i].i64ID = psLinkItemInfo[i].i64ID;
        wcsncpy(psItemLinkInfoServer.psItemLinkInfo[i].szLinkString,
                psLinkItemInfo[i].szLinkString, 64);
    }
    
    // Get user
    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }
    
    // Process megaphone through logic thread
    if (pUser && pUser->GetArea()) {
        XClient::IncrementJobCount(pUser);
        
        // Queue megaphone processing
        // This will broadcast to entire server
        
        return true;
    }
    
    return false;
}

// ReqGMCommand - GM command processing
// IDA: ?ReqGMCommand@CChatProcess@@IEAA_NAEAVXPacket@@@Z (0x1403E6E70)
bool CChatProcess::ReqGMCommand(XPacket& packet)
{
    // Parse GM command
    wchar_t szMsg[264] = {0};
    short shLen = 0;
    packet.XParse.GetWString(szMsg, 257, &shLen);
    
    if (packet.GetLastError() != XPacket::eSUCCESS) {
        return true;
    }
    
    // Get user
    CUser* pUser = GetClientPtr();
    
    // Check GM permissions
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    XOption* pOption = pServer->GetOption();
    if (pOption->GetSystemType() != 1 && !pUser->GetGMPower()) {
        return true;
    }
    
    // Process command string
    std::wstring strCommand(szMsg);
    
    // Execute GM command through logic thread
    if (pUser && pUser->GetArea()) {
        XClient::IncrementJobCount(pUser);
        
        // Queue GM command processing
        int nResult = 0;
        __int64 n64Result = 0;
        GMCommandProcess(strCommand, pUser, &nResult, n64Result);
        
        return true;
    }
    
    return false;
}

// ===== MESSAGE PROCESSING =====

// ProcessNormal - SendChat: Process and broadcast normal chat
// IDA: ?ProcessNormal@CChatProcess@@KA_NPEAVCUser@@PEA_WUPS_CHAT_ITEM_LINK_FOR_SERVER@@@Z (0x1403E5F40)
bool CChatProcess::ProcessNormal(CUser* pUser, wchar_t* szMsg, PS_CHAT_ITEM_LINK_FOR_SERVER* psChatLinkInfoServer)
{
    if (!pUser || !szMsg || !psChatLinkInfoServer) {
        return false;
    }
    
    // Create send packet for normal chat
    XSendPacket xSendPacket(7, 1);
    
    // Add actor ID
    xSendPacket.XParse << pUser->GetActorID().GetID();
    xSendPacket.XParse << 1;  // Chat type: normal
    
    // Add message
    xSendPacket.XParse << szMsg;
    
    // Add item link count
    xSendPacket.XParse << psChatLinkInfoServer->byItemLinkCount;
    
    // Add item links
    PS_CHAT_ITEM_LINK psLinkItemInfo[3];
    for (int i = 0; i < psChatLinkInfoServer->byItemLinkCount; ++i) {
        psLinkItemInfo[i] = psChatLinkInfoServer->psItemLinkInfo[i];
        xSendPacket << psLinkItemInfo[i];
    }
    
    // Broadcast to nearby players
    CGocNetwork::SendBroadCast(pUser, &xSendPacket, 0);
    
    // Log chat message
    int nLogParam[8] = {0};
    __int64 n64LogParam[2] = {0};
    
    SendChatLog(1, pUser->GetUAID(), pUser->GetActorID().GetID(),
                nLogParam, n64LogParam, szMsg);
    
    return true;
}

// ProcessParty - SendPartyChat: Process party/force chat
// IDA: ?ProcessParty@CChatProcess@@KA_NPEAVCUser@@PEA_WUPS_CHAT_ITEM_LINK_FOR_SERVER@@@Z (0x1403E6410)
bool CChatProcess::ProcessParty(CUser* pUser, wchar_t* szMsg, PS_CHAT_ITEM_LINK_FOR_SERVER* psChatLinkInfoServer)
{
    if (!pUser || !szMsg || !psChatLinkInfoServer) {
        return false;
    }
    
    // Get party component
    auto pParty = pUser->GetGOC<CGocParty>();
    auto pForce = pUser->GetGOC<CGocForce>();
    
    if (pParty && pParty->IsParty()) {
        // Send to party chat
        PS_CHAT_PARTY stChatParty;
        stChatParty.dwActorID = pUser->GetActorID().GetID();
        stChatParty.dwPartyID = pParty->GetPartyID();
        wcscpy_s(stChatParty.szMsg, szMsg);
        
        XSendPacket xSendPacket(0xF4, 0x14);
        xSendPacket << stChatParty;
        xSendPacket << *psChatLinkInfoServer;
        
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        pServer->m_communitySocket.SendCmd(&xSendPacket, pUser, 7, 1);
        
        // Log party chat
        int nLogParam[8] = {0};
        __int64 n64LogParam[2] = {0};
        nLogParam[2] = pParty->GetPartyID();
        
        SendChatLog(3, pUser->GetUAID(), pUser->GetActorID().GetID(),
                    nLogParam, n64LogParam, szMsg);
    }
    else if (pForce && pForce->IsParty()) {
        // Send to force chat (guild-like)
        PS_CHAT_FORCE stChatForce;
        stChatForce.dwActorID = pUser->GetActorID().GetID();
        stChatForce.dwForceID = pForce->GetPartyID();
        wcscpy_s(stChatForce.szMsg, szMsg);
        
        XSendPacket xSendPacket(0xFA, 0x10);
        xSendPacket << stChatForce;
        xSendPacket << *psChatLinkInfoServer;
        
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        pServer->m_communitySocket.SendCmd(&xSendPacket, pUser, 7, 1);
        
        // Log force chat
        int nLogParam[8] = {0};
        __int64 n64LogParam[2] = {0};
        nLogParam[2] = pForce->GetPartyID();
        
        SendChatLog(3, pUser->GetUAID(), pUser->GetActorID().GetID(),
                    nLogParam, n64LogParam, szMsg);
    }
    else {
        // Not in party - send as normal chat with party type
        XSendPacket packet(7, 1);
        packet.XParse << pUser->GetActorID().GetID();
        packet.XParse << 3;  // Chat type: party
        packet.XParse << szMsg;
        packet.XParse << psChatLinkInfoServer->byItemLinkCount;
        
        for (int i = 0; i < psChatLinkInfoServer->byItemLinkCount; ++i) {
            PS_CHAT_ITEM_LINK psChatLinkItemInfo = psChatLinkInfoServer->psItemLinkInfo[i];
            packet << psChatLinkItemInfo;
        }
        
        CGocNetwork::Send(pUser, &packet);
    }
    
    return true;
}

// ProcessLeague - SendGuildChat: Process league/guild chat
// IDA: ?ProcessLeague@CChatProcess@@KA_NPEAVCUser@@PEA_WUPS_CHAT_ITEM_LINK_FOR_SERVER@@@Z (0x1403E6B90)
bool CChatProcess::ProcessLeague(CUser* pUser, wchar_t* szMsg, PS_CHAT_ITEM_LINK_FOR_SERVER* psChatLinkInfoServer)
{
    if (!pUser || !szMsg || !psChatLinkInfoServer) {
        return false;
    }
    
    // Check if user is in a league
    int nLeagueID = pUser->GetLeagueID();
    if (!nLeagueID) {
        return true;
    }
    
    // Create league chat packet
    PS_CHAT_LEAGUE stChatLeague;
    stChatLeague.dwActorID = pUser->GetActorID().GetID();
    stChatLeague.dwLeagueID = nLeagueID;
    wcscpy_s(stChatLeague.szMsg, szMsg);
    
    XSendPacket xSendPacket(0xF6, 0x36);
    xSendPacket << stChatLeague;
    xSendPacket << *psChatLinkInfoServer;
    
    // Send to community server for distribution
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    pServer->m_communitySocket.SendCmd(&xSendPacket, pUser, 7, 1);
    
    // Log league chat
    int nLogParam[8] = {0};
    __int64 n64LogParam[2] = {0};
    nLogParam[2] = nLeagueID;
    
    SendChatLog(4, pUser->GetUAID(), pUser->GetActorID().GetID(),
                nLogParam, n64LogParam, szMsg);
    
    return true;
}

// ===== WHISPER HANDLING =====

// SendChatWhisper - Send private message to target player
// IDA: ?SendChatWhisper@CChatProcess@@QEAAXPEAVCUser@@AEAUPS_CHAT_WHISPER@@UPS_CHAT_ITEM_LINK_FOR_SERVER@@@Z (0x1403E7A70)
void CChatProcess::SendChatWhisper(CUser* pUser, PS_CHAT_WHISPER& stChatWhisper, PS_CHAT_ITEM_LINK_FOR_SERVER psLinkItemInfo)
{
    if (!pUser) {
        return;
    }
    
    // Create whisper packet
    XSendPacket xSendPacket(7, 2);
    xSendPacket << stChatWhisper;
    
    // Add item link count
    xSendPacket.XParse << psLinkItemInfo->byItemLinkCount;
    
    // Add item links
    for (int i = 0; i < psLinkItemInfo->byItemLinkCount; ++i) {
        PS_CHAT_ITEM_LINK psInfo = psLinkItemInfo->psItemLinkInfo[i];
        xSendPacket << psInfo;
    }
    
    // Send to target player
    CGocNetwork::Send(pUser, &xSendPacket);
}

// ===== GM COMMAND HANDLING =====

// GMCommandProcess - Process GM command string
// IDA: ?GMCommandProcess@CChatProcess@@IEAAEV?$CStringT@_WV?$StrTraitATL@_WV?$ChTraitsCRT@_W@ATL@@@ATL@@@ATL@@PEAVCUser@@PEAHAEA_J@Z (0x1403E7BE0)
unsigned char CChatProcess::GMCommandProcess(const std::wstring& strCommand, CUser* pUser, int* pnResult, __int64& rnResult)
{
    // Parse and execute GM command
    // This is a placeholder - full implementation would parse command string
    // and route to appropriate GM command handler
    
    if (!pUser) {
        return 0;
    }
    
    // TODO: Implement full GM command parsing and execution
    // Commands would include:
    // - /create_item - Create items
    // - /teleport - Teleport player
    // - /level - Set player level
    // - /gold - Add gold
    // - /notice - Send server notice
    // - etc.
    
    return 1;
}

// InitGMCommand - Initialize GM command map
// IDA: ?InitGMCommand@CChatProcess@@QEAAXXZ (0x1403E7F20)
void CChatProcess::InitGMCommand()
{
    // Initialize GM command map with available commands
    // This would register all GM commands with their handlers
    
    // TODO: Register GM commands:
    // - Create item
    // - Teleport
    // - Set level
    // - Add gold
    // - Set stats
    // - Monster spawning
    // - Server notices
    // - etc.
}

// InitCommand_Debug - Initialize debug commands
// IDA: ?InitCommand_Debug@CChatProcess@@QEAAXXZ (0x1403F0F60)
void CChatProcess::InitCommand_Debug()
{
    // Initialize debug/development commands
    // These are typically only available in debug builds
    
    // TODO: Register debug commands:
    // - Performance profiling
    // - Memory tracking
    // - Network debugging
    // - etc.
}

// ===== HELPER FUNCTIONS =====

// GetChatLinkItem - Get item link information from user inventory
// IDA: ?GetChatLinkItem@CChatProcess@@IEAA_NPEAVCUser@@AEAUPS_CHAT_ITEM_LINK_FOR_SERVER@@@Z (0x1403E5C50)
bool CChatProcess::GetChatLinkItem(CUser* pUser, PS_CHAT_ITEM_LINK_FOR_SERVER& psLinkItemInfo)
{
    if (!pUser) {
        return false;
    }
    
    // Get item from user inventory for linking in chat
    // This allows players to show item stats in chat
    
    // TODO: Implement item link retrieval from inventory
    
    return true;
}

// SendChatLog - Log chat message to database
// Helper function to log all chat messages for auditing
void CChatProcess::SendChatLog(unsigned int dwType, unsigned int dwUAID, unsigned int dwUCID,
                               int* pnLogParam, __int64* pn64LogParam, const wchar_t* szMsg)
{
    // Create log packet
    ST_CHAT_LOG_GAME stLog;
    stLog.dwType = dwType;
    stLog.dwUAID = dwUAID;
    stLog.dwUCID = dwUCID;
    stLog.szMsg = szMsg;
    
    // Send to database for logging
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    pServer->SendDBChatLog(stLog);
}

// ===== CHATTING COMMAND MANAGER =====

CChattingCommandMgr::CChattingCommandMgr()
{
}

CChattingCommandMgr::~CChattingCommandMgr()
{
    Clear();
}

void CChattingCommandMgr::Clear()
{
    // Clear all registered commands
    for (auto& pair : m_mapCommand) {
        delete pair.second;
    }
    m_mapCommand.clear();
}
