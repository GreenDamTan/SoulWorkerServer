#pragma once

// Chat process handler for GameServer
// Handles packet processing for all chat and communication systems
// 
// This process handles:
// - Normal chat (SendChat)
// - Whisper/private messages (SendWhisper)
// - Party chat (SendPartyChat)
// - Guild/League chat (SendGuildChat)
// - Trade chat
// - Megaphone broadcasts
// - Notice/system broadcasts
// - GM commands
//
// Based on IDA analysis from GameServer.exe (CChatProcess class)

#include <string>
#include <map>
#include <functional>

// Forward declarations
class CUser;
class XPacket;
struct PS_CHAT_WHISPER;
struct PS_CHAT_ITEM_LINK_FOR_SERVER;
struct TB_CHATTINGCOMMAND;

// Chat command manager for GM commands
class CChattingCommandMgr
{
public:
    CChattingCommandMgr();
    ~CChattingCommandMgr();
    
    void Clear();
    
private:
    std::map<std::wstring, TB_CHATTINGCOMMAND*> m_mapCommand;
};

// Chat process class - handles packet processing for all chat systems
class CChatProcess : public TXProcess<CUser>
{
public:
    CChatProcess();
    virtual ~CChatProcess();
    
    // Main packet parser
    bool Parse(XPacket& packet) override;
    
    // ===== CHAT MESSAGE HANDLERS =====
    
    // Normal chat (broadcast to nearby players)
    bool ReqNormalChatting(XPacket& packet);       // SendChat - normal chat message
    
    // Whisper (private message to specific player)
    bool ReqChatWhisper(XPacket& packet);          // SendWhisper - private message
    
    // Party chat
    bool ReqChatParty(XPacket& packet);            // SendPartyChat - party/force message
    
    // Trade chat (trade channel)
    bool ReqChatTrade(XPacket& packet);            // Trade chat message
    
    // Notice (system broadcast)
    bool ReqChatNotice(XPacket& packet);           // BroadcastMessage - system notice
    
    // Megaphone (server-wide broadcast)
    bool ReqChatMegaPhone(XPacket& packet);        // Megaphone broadcast
    
    // GM command handler
    bool ReqGMCommand(XPacket& packet);            // GM command processing
    
    // ===== MESSAGE PROCESSING =====
    
    // Process normal chat message
    static bool ProcessNormal(CUser* pUser, wchar_t* szMsg, PS_CHAT_ITEM_LINK_FOR_SERVER* psChatLinkInfoServer);
    
    // Process party chat message  
    static bool ProcessParty(CUser* pUser, wchar_t* szMsg, PS_CHAT_ITEM_LINK_FOR_SERVER* psChatLinkInfoServer);
    
    // Process league/guild chat message
    static bool ProcessLeague(CUser* pUser, wchar_t* szMsg, PS_CHAT_ITEM_LINK_FOR_SERVER* psChatLinkInfoServer);
    
    // ===== WHISPER HANDLING =====
    
    // Send whisper message to target user
    void SendChatWhisper(CUser* pUser, PS_CHAT_WHISPER& stChatWhisper, PS_CHAT_ITEM_LINK_FOR_SERVER psLinkItemInfo);
    
    // ===== GM COMMAND HANDLING =====
    
    // Process GM command string
    unsigned char GMCommandProcess(const std::wstring& strCommand, CUser* pUser, int* pnResult, __int64& rnResult);
    
    // Initialize GM command map
    void InitGMCommand();
    
    // Initialize debug commands
    void InitCommand_Debug();
    
    // ===== HELPER FUNCTIONS =====
    
    // Get item link information from user inventory
    bool GetChatLinkItem(CUser* pUser, PS_CHAT_ITEM_LINK_FOR_SERVER& psLinkItemInfo);
    
    // Send chat log to database
    static void SendChatLog(unsigned int dwType, unsigned int dwUAID, unsigned int dwUCID, 
                           int* pnLogParam, __int64* pn64LogParam, const wchar_t* szMsg);
    
private:
    // Chat command manager for GM commands
    CChattingCommandMgr m_chatCommandMgr;
};
